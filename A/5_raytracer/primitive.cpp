#include"primitive.h"
#include<sstream>
#include<cstdio>
#include<string>
#include<cmath>
#include<iostream>
#include<cstdlib>

#define ran() ( double( rand() % 32768 ) / 32768 )

const int BEZIER_MAX_DEGREE = 5;
const int Combination[BEZIER_MAX_DEGREE + 1][BEZIER_MAX_DEGREE + 1] =
        {0, 0, 0, 0, 0, 0,
         1, 1, 0, 0, 0, 0,
         1, 2, 1, 0, 0, 0,
         1, 3, 3, 1, 0, 0,
         1, 4, 6, 4, 1, 0,
         1, 5, 10, 10, 5, 1
        };

const int MAX_COLLIDE_TIMES = 10;
const int MAX_COLLIDE_RANDS = 10;
const int RANDOM_INITIALIZATION = 6;
const int MAX_NEWTON_ITERATION = 20;
const double RAND_AMP = 0.5;
const double LEARNING_RATE = 0.5;


std::pair<double, double> ExpBlur::GetXY() {
    double x, y;
    x = ran();
    x = pow(2, x) - 1;
    y = rand();
    return std::pair<double, double>(x * cos(y), x * sin(y));
}

Material::Material() {
    color = absor = Color();
    refl = refr = 0;
    diff = spec = 0;
    rindex = 0;
    drefl = 0;
    texture = NULL;
    blur = new ExpBlur();
}

void Material::Input(std::string var, std::stringstream &fin) {
    if (var == "color=") color.Input(fin);
    if (var == "absor=") absor.Input(fin);
    if (var == "refl=") fin >> refl;
    if (var == "refr=") fin >> refr;
    if (var == "diff=") fin >> diff;
    if (var == "spec=") fin >> spec;
    if (var == "drefl=") fin >> drefl;
    if (var == "rindex=") fin >> rindex;
    if (var == "texture=") {
        std::string file;
        fin >> file;
        texture = new Bmp;
        texture->Input(file);
    }
    if (var == "blur=") {
        std::string blurname;
        fin >> blurname;
        if (blurname == "exp")
            blur = new ExpBlur();
    }
}

Primitive::Primitive() {
    sample = rand();
    material = new Material;
    next = NULL;
}

Primitive::Primitive(const Primitive &primitive) {
    *this = primitive;
    material = new Material;
    *material = *primitive.material;
}

Primitive::~Primitive() {
    delete material;
}

void Primitive::Input(std::string var, std::stringstream &fin) {
    material->Input(var, fin);
}

Sphere::Sphere() : Primitive() {
    De = Vector3(0, 0, 1);
    Dc = Vector3(0, 1, 0);
}

void Sphere::Input(std::string var, std::stringstream &fin) {
    if (var == "O=") O.Input(fin);
    if (var == "R=") fin >> R;
    if (var == "De=") De.Input(fin);
    if (var == "Dc=") Dc.Input(fin);
    Primitive::Input(var, fin);
}

CollidePrimitive Sphere::Collide(Vector3 ray_O, Vector3 ray_V) {
    ray_V = ray_V.GetUnitVector(); // 光线方向
    Vector3 P = ray_O - O; // 球心到发射点的向量
    double b = -P.Dot(ray_V); // b = ||P||^2 * cos^2(theta)
    double det = b * b - P.Module2() + R * R; // det = R^2 - ( ||P|| * sin(theta) )^2
    CollidePrimitive ret;

    if (det > EPS) {
        // det > 0 说明射线与球有两个交点
        det = sqrt(det);
        // x1 < x2
        double x1 = b - det, x2 = b + det;

        // x1 < x2 < 0 说明球在射线的反方向相交
        if (x2 < EPS) return ret;
        if (x1 > EPS) {
            // 0 < x1 < x2 说明球与射线有两个交点，x1是第一个交点 且 交点是球面的正面
            ret.dist = x1;
            ret.front = true;
        } else {
            // x1 < 0 < x2 说明球与射线在正方向上只有一个交点 且 交点是球面的背面
            ret.dist = x2;
            ret.front = false;
        }
    } else
        // det <= 0 说明射线与球有两个交点
        return ret;

    ret.C = ray_O + ray_V * ret.dist; // 交点
    ret.N = (ret.C - O).GetUnitVector(); // 交点所在面为正面时的法向量
    if (ret.front == false) ret.N = -ret.N; // 交点在背面，法向量取负
    ret.isCollide = true;
    ret.collide_primitive = this;
    return ret;
}

Color Sphere::GetTexture(Vector3 crash_C) {
    Vector3 I = (crash_C - O).GetUnitVector(); // 球心到交点的方向向量
    double a = acos(-I.Dot(De));
    double b = acos(std::min(std::max(I.Dot(Dc) / sin(a), -1.0), 1.0));
    double u = a / PI, v = b / 2 / PI;
    if (I.Dot(Dc * De) < 0) v = 1 - v;
    return material->texture->GetSmoothColor(u, v);
}


void Plane::Input(std::string var, std::stringstream &fin) {
    if (var == "N=") N.Input(fin);
    if (var == "R=") fin >> R;
    if (var == "Dx=") Dx.Input(fin);
    if (var == "Dy=") Dy.Input(fin);
    Primitive::Input(var, fin);
}

CollidePrimitive Plane::Collide(Vector3 ray_O, Vector3 ray_V) {
    ray_V = ray_V.GetUnitVector(); // 光线方向
    N = N.GetUnitVector(); // 平面单位法向量
    double d = N.Dot(ray_V); // cos(theta)
    CollidePrimitive ret;
    // cos(theta) = 0 说明光线与平面平行，无交点
    if (fabs(d) < EPS) return ret;
    double l = (N * R - ray_O).Dot(N) / d; // R * N 是平面上的一个点 (交点p - 平面上另一点p') . N = 0
    if (l < EPS) return ret; // 若l < 0，则说明交点在射线反方向

    ret.dist = l;
    ret.front = (d < 0); // cos(theta) < 0 => theta > 90°
    ret.C = ray_O + ray_V * ret.dist; // 交点
    ret.N = (ret.front) ? N : -N;
    ret.isCollide = true;
    ret.collide_primitive = this;
    return ret;
}

Color Plane::GetTexture(Vector3 crash_C) {
    double u = crash_C.Dot(Dx) / Dx.Module2();
    double v = crash_C.Dot(Dy) / Dy.Module2();
    return material->texture->GetSmoothColor(u, v);
}

void Square::Input(std::string var, std::stringstream &fin) {
    if (var == "O=") O.Input(fin);
    if (var == "Dx=") Dx.Input(fin);
    if (var == "Dy=") Dy.Input(fin);
    Primitive::Input(var, fin);
}

CollidePrimitive Square::Collide(Vector3 ray_O, Vector3 ray_V) {
    //TODO: NEED TO IMPLEMENT
    ray_V = ray_V.GetUnitVector(); // 光线方向
    Vector3 xDir = Dx.GetUnitVector(), yDir = Dy.GetUnitVector(); // 横纵坐标轴
    double length = Dx.Module(), width = Dy.Module();

    Vector3 N = (xDir * yDir).GetUnitVector(); // 平面单位法向量
    double d = N.Dot(ray_V);
    CollidePrimitive ret;
    // cos(theta) = 0 说明光线与平面平行，无交点
    if (fabs(d) < EPS) return ret;
    double t = (O - ray_O).Dot(N) / d; // 交点p = ray_O + t * ray_V
    // 若t < 0，则说明交点在射线反方向
    if (t < EPS) return ret;

    Vector3 collidePoint = ray_O + t * ray_V; // 交点p
    Vector3 OP = collidePoint - O;
    double x = OP.Dot(xDir), y = OP.Dot(yDir);

    if (x + length < EPS || x - length > EPS || y + width < EPS || y - width > EPS) return ret; // 交点在矩形外部

    ret.dist = t;
    ret.front = (d < 0); // cos(theta) < 0 => theta > 90°
    ret.C = collidePoint; // 交点
    ret.N = (ret.front) ? N : -N;
    ret.isCollide = true;
    ret.collide_primitive = this;
    return ret;
}

Color Square::GetTexture(Vector3 crash_C) {
    double u = (crash_C - O).Dot(Dx) / Dx.Module2() / 2 + 0.5;
    double v = (crash_C - O).Dot(Dy) / Dy.Module2() / 2 + 0.5;
    return material->texture->GetSmoothColor(u, v);
}

void Cylinder::Input(std::string var, std::stringstream &fin) {
    if (var == "O1=") O1.Input(fin);
    if (var == "O2=") O2.Input(fin);
    if (var == "R=") fin >> R;
    Primitive::Input(var, fin);
}

CollidePrimitive Cylinder::Collide(Vector3 ray_O, Vector3 ray_V) {
    //TODO: NEED TO IMPLEMENT
    CollidePrimitive collisions[3] = {
            CollideAtBase(ray_O, ray_V, O1), // 底面
            CollideAtBase(ray_O, ray_V, O2), // 顶面
            CollideAtSide(ray_O, ray_V)         // 侧面
    };

    CollidePrimitive ret;
    for (auto &collision: collisions) {
        if (collision.isCollide && collision.dist < ret.dist) {
            ret = collision;
        }
    }
    return ret;
}

Color Cylinder::GetTexture(Vector3 crash_C) {
    //TODO: NEED TO IMPLEMENT0
    double u = 0.5, v = 0.5;
    Vector3 De = (O2 - O1).GetUnitVector(); // 圆柱向上的方向
    Vector3 Dc = De.GetAnVerticalVector(); // 圆柱某一个横向方向

    double sqDist1 = crash_C.Distance2(O1); // 交点与底面圆心距离的平方
    double sqDist2 = crash_C.Distance2(O2); // 交点与顶面圆心距离的平方

    if (sqDist1 - R * R > EPS && sqDist2 - R * R > EPS) {
        // 交点在圆柱侧面
        double HEIGHT = O1.Distance(O2); // 圆柱高度

        Vector3 O1C = crash_C - O1; // 底面圆心到交点的向量
        double height = De.Dot(O1C); // 交点到底面的垂直距离

        Vector3 projectPoint = crash_C - height * De;
        Vector3 O1P = (projectPoint - O1).GetUnitVector();
        double theta = acos(std::min(std::max(O1P.Dot(Dc), -1.0), 1.0));

        u = theta / 2 / PI, v = height / HEIGHT;
    } else if (sqDist1 - R * R > EPS) {
        // 交点在圆柱顶面
        Vector3 O2C = (crash_C - O2).GetUnitVector();
        double theta = acos(std::min(std::max(O2C.Dot(Dc), -1.0), 1.0));
        double radius = crash_C.Distance(O2);
        u = theta / 2 / PI, v = radius / R;
    } else {
        // 交点在圆柱底面
        Vector3 O1C = (crash_C - O1).GetUnitVector();
        double theta = acos(std::min(std::max(O1C.Dot(Dc), -1.0), 1.0));
        double radius = crash_C.Distance(O1);
        u = theta / 2 / PI, v = radius / R;
    }
    return material->texture->GetSmoothColor(u, v);
}

CollidePrimitive Cylinder::CollideAtSide(Vector3 ray_O, Vector3 ray_V) {
    ray_V = ray_V.GetUnitVector(); // 光线方向
    Vector3 N1 = (O1 - O2).GetUnitVector(); // 圆柱底面单位法向量，方向朝底面外部
    Vector3 N2 = (O2 - O1).GetUnitVector(); // 圆柱顶面单位法向量，方向朝顶面外部

    // [ O_1P x (-N1) ]^2 = R^2
    Vector3 f1 = (ray_O - O1) * (-N1);
    Vector3 g1 = ray_V * (-N1);
    double a = g1.Dot(g1);
    double b = 2 * f1.Dot(g1);
    double c = f1.Dot(f1) - R * R;
    double det = b * b - 4 * a * c; // 二次方程根中的delta

    CollidePrimitive ret;
    double side_dist = 0.0;
    bool side_front = false;
    if (det > EPS) {
        // det > 0 说明射线与圆柱侧面有两个交点
        det = sqrt(det);
        // t_side1 < t_side2
        double t_side1 = (-b - det) / (2 * a), t_side2 = (-b + det) / (2 * a);

        if (t_side2 < EPS) {
            // t_side1 < t_side2 < 0 说明圆柱侧面在射线的反方向相交
            return ret;
        } else {
            // 圆柱侧面在射线的正方向上至少有一个交点
            if (t_side1 > EPS) {
                // 0 < t_side1 < t_side2 说明圆柱侧面与射线有两个交点，x1是第一个交点 且 交点是圆柱侧面的正面

                // 判断两个交点是否都在圆柱的高度范围内

                // 靠近射线源点的交点1
                // ----------------
                Vector3 sideCollidePoint1 = ray_O + ray_V * t_side1;
                Vector3 O1P1 = sideCollidePoint1 - O1; // 底面圆心到交点1的向量
                Vector3 O2P1 = sideCollidePoint1 - O2; // 顶面圆心到交点1的向量
                double thetaO1P1 = O1P1.Dot(N1), thetaO2P1 = O2P1.Dot(N2); // N1和N2都朝向圆柱的外部

                // 远离射线源点的交点2
                // ---------------
                Vector3 sideCollidePoint2 = ray_O + ray_V * t_side2;
                Vector3 O1P2 = sideCollidePoint2 - O1; // 底面圆心到交点2的向量
                Vector3 O2P2 = sideCollidePoint2 - O2; // 顶面圆心到交点2的向量
                double thetaO1P2 = O1P2.Dot(N1), thetaO2P2 = O2P2.Dot(N2); // N1和N2都朝向圆柱的外部

                if (thetaO1P1 < EPS && thetaO2P1 < EPS) {
                    // 交点1 在圆柱的高度范围内
                    side_dist = t_side1;
                    side_front = true;
                } else if (thetaO1P2 < EPS && thetaO2P2 < EPS) {
                    // 交点2 在圆柱的高度范围内
                    side_dist = t_side2;
                    side_front = false;
                } else {
                    // 两个交点都不在圆柱的高度范围内
                    return ret;
                }
            } else {
                // t_side1 < 0 < t_side2 说明圆柱侧面与射线在正方向上只有一个交点 且 交点是圆柱侧面的背面
                side_dist = t_side2;
                side_front = false;
                // 判断交点是否在圆柱的高度范围内
                Vector3 sideCollidePoint = ray_O + ray_V * side_dist; // 交点
                Vector3 O1P = sideCollidePoint - O1; // 底面圆心到交点的向量
                Vector3 O2P = sideCollidePoint - O2; // 顶面圆心到交点的向量
                double theta1 = O1P.Dot(N1), theta2 = O2P.Dot(N2); // N1和N2都朝向圆柱的外部
                if (theta1 > EPS || theta2 > EPS) {
                    return ret;
                }
            }
        }
    } else {
        // det <= 0 说明射线与圆柱侧面没有交点
        return ret;
    }

    ret.dist = side_dist;
    ret.front = side_front;
    ret.C = ray_O + ray_V * ret.dist; // 交点
    ret.N = ((ret.C - O1) - (ret.C - O1).Dot(-N1) * (-N1)).GetUnitVector(); // 交点所在面为正面时的法向量
    if (ret.front == false) ret.N = -ret.N; // 交点在背面，法向量取负
    ret.isCollide = true;
    ret.collide_primitive = this;
    return ret;
}

CollidePrimitive Cylinder::CollideAtBase(Vector3 ray_O, Vector3 ray_V, Vector3 O) {
    ray_V = ray_V.GetUnitVector(); // 光线方向
    Vector3 N;
    if ((O - O1).IsZeroVector()) {
        N = (O1 - O2).GetUnitVector(); // 圆柱底面单位法向量，方向朝底面外部
    } else {
        N = (O2 - O1).GetUnitVector(); // 圆柱顶面单位法向量，方向朝顶面外部
    }
    // cos(theta1) = 0 说明光线与圆柱底面平行，无交点
    double d = N.Dot(ray_V);
    CollidePrimitive ret;
    // cos(theta) = 0 说明光线与平面平行，无交点
    if (fabs(d) < EPS) return ret;
    double t = (O - ray_O).Dot(N) / d; // 交点p = ray_O + t * ray_V
    // 若t < 0，则说明交点在射线反方向
    if (t < EPS) return ret;

    Vector3 collidePoint = ray_O + t * ray_V; // 交点p
    double sqDist1 = collidePoint.Distance2(O); // 交点和底面圆心距离的平方
    // 交点在底面圆的外面
    if (sqDist1 - R * R > EPS) return ret;

    ret.dist = t;
    ret.front = (d < 0); // cos(theta) < 0 => theta > 90°
    ret.C = collidePoint; // 交点
    ret.N = (ret.front) ? N : -N;
    ret.isCollide = true;
    ret.collide_primitive = this;
    return ret;
}

void Bezier::Input(std::string var, std::stringstream &fin) {
    if (var == "O1=") O1.Input(fin);
    if (var == "O2=") O2.Input(fin);
    if (var == "P=") {
        degree++;
        double newR, newZ;
        fin >> newZ >> newR;
        R.push_back(newR);
        Z.push_back(newZ);
    }
    if (var == "Cylinder") {
        double maxR = 0;
        for (int i = 0; i < R.size(); i++)
            if (R[i] > maxR)
                maxR = R[i];
        boundingCylinder = new Cylinder(O1, O2, maxR);
        N = (O1 - O2).GetUnitVector();
        Nx = N.GetAnVerticalVector();
        Ny = N * Nx;
    }
    Primitive::Input(var, fin);
}

CollidePrimitive Bezier::Collide(Vector3 ray_O, Vector3 ray_V) {
    //TODO: NEED TO IMPLEMENT
    ray_V = ray_V.GetUnitVector(); // 光线方向
    CollidePrimitive ret;

    // 先判断光线是否交于包围圆柱
    CollidePrimitive boundCollisions[3] = {
            boundingCylinder->CollideAtBase(ray_O, ray_V, O1), // 底面
            boundingCylinder->CollideAtBase(ray_O, ray_V, O2), // 顶面
            boundingCylinder->CollideAtSide(ray_O, ray_V)         // 侧面
    };

    // 包围圆柱的碰撞个数
    int collisionCount = 0;
    for (auto &boundCollision : boundCollisions) {
        if (boundCollision.isCollide) {
            collisionCount++;
        }
    }
    if (collisionCount < 1) {
        return ret;
    }

    for (int i = 0; i < 3; i++) {
        if (!boundCollisions[i].isCollide) continue;

        // 光线交于底面圆
        if (i == 0 && (boundCollisions[i].C - O1).Module() < R[0] + EPS) {
            ret.dist = boundCollisions[i].dist;
            ret.front = boundCollisions[i].front;
            ret.C = boundCollisions[i].C; // 交点
            ret.N = boundCollisions[i].N;
            ret.isCollide = true;
            ret.collide_primitive = this;
        }

        // 光线交于顶面圆
        if (i == 1 && (boundCollisions[i].C - O2).Module() < R[R.size() - 1] + EPS) {
            ret.dist = boundCollisions[i].dist;
            ret.front = boundCollisions[i].front;
            ret.C = boundCollisions[i].C; // 交点
            ret.N = boundCollisions[i].N;
            ret.isCollide = true;
            ret.collide_primitive = this;
        }

        // 光线交于侧面
        Vector3 O1C = boundCollisions[i].C - O1;
        double t0 = boundCollisions[i].dist;
        double u0 = O1C.Dot(N) / (O2 - O1).Module(); // 高度百分比
        double theta0 = atan2(O1C.Dot(Ny), O1C.Dot(Nx));

        double t = t0, u = u0, theta = theta0;
        CollidePrimitive collision = CollideAtBezierCurve(ray_O, ray_V, t, u, theta);
        if (collision.isCollide && collision.dist < ret.dist) {
            ret = collision;
        }

        for (int j = 0; j < RANDOM_INITIALIZATION / collisionCount; j++) {
            t = t0 * ((1 - RAND_AMP) + 2 * RAND_AMP * ran());
            u = ran();
            theta = theta0 * ((1 - RAND_AMP) + 2 * RAND_AMP * ran());// -PI + 2 * PI * i / (double)RANDOM_INITIALIZATION;

            collision = Bezier::CollideAtBezierCurve(ray_O, ray_V, t, u, theta);
            if (collision.isCollide && collision.dist < ret.dist) {
                ret = collision;
            }
        }
    }

    return ret;
}

CollidePrimitive Bezier::CollideAtBezierCurve(Vector3 ray_O, Vector3 ray_V, double t, double u, double theta) {
    CollidePrimitive ret;
    Eigen::Vector3d args(t, u, theta);
    bool isZero = false;

    // 牛顿迭代法求 函数F(t,u,θ) 的零点
    for (int i = 0; i < MAX_NEWTON_ITERATION; i++) {
        t = args.x(), u = args.y(), theta = args.z();

        if (u < -0.5 || u > 1.5) break;

        Eigen::Vector3d F = Bezier::F(t, u, theta, ray_O, ray_V).ToEigenLibForm();
        Eigen::Matrix3d dF = Bezier::dF(t, u, theta, ray_V);

        if (F.norm() < EPS) {
            isZero = true;
            break;
        }

        args = args - LEARNING_RATE * (dF.inverse() * F);
    }

    if (!isZero) return ret;
    if (t < -EPS) return ret;
    if (u < -EPS || u > 1 + EPS) return ret;

    Vector3 dS_u = dP(u, Z) * (O2 - O1) +
                   dP(u, R) * (cos(theta) * Nx + sin(theta) * Ny);
    Vector3 dS_theta = P(u, R) * (-sin(theta) * Nx + cos(theta) * Ny);

    ret.dist = t;
    ret.C = C(t, ray_O, ray_V);
    ret.N = (dS_theta * dS_u).GetUnitVector();
    ret.front = (ret.N).Dot(ray_V) < 0;
    if (ret.front == false) ret.N = -ret.N;
    ret.isCollide = true;
    ret.collide_primitive = this;
    return ret;
}

Color Bezier::GetTexture(Vector3 crash_C) {
    double u = 0.5, v = 0.5;
    //TODO: NEED TO IMPLEMENT
    return material->texture->GetSmoothColor(u, v);
}

double Bezier::B(double u, int n, int i) {
    // B_i,n(u) = C_n,i * u^i * (1-u)^(n-i)
    return Combination[n][i] * pow(u, i) * pow(1 - u, n - i);
}

double Bezier::P(double u, std::vector<double> ctrlPointsComponents) {
    // b_n(u) = Σb_j * B_j,n(u)
    double componentValue = 0.0;
    for (int i = 0; i < degree + 1; i++) {
        componentValue += B(u, degree, i) * ctrlPointsComponents[i];
    }
    return componentValue;
}

double Bezier::dP(double u, std::vector<double> ctrlPointsComponents) {
    double componentValue = 0.0;
    for (int i = 0; i < degree; i++) {
        componentValue += B(u, degree - 1, i) * (ctrlPointsComponents[i + 1] - ctrlPointsComponents[i]);
    }
    componentValue *= degree;
    return componentValue;
}

Vector3 Bezier::C(double t, Vector3 ray_O, Vector3 ray_V) {
    return ray_O + t * ray_V;
}

Vector3 Bezier::S(double u, double theta) {
    return O1 +
           P(u, Z) * (O2 - O1) +                           // 在包围圆柱高度上的投影
           P(u, R) * (cos(theta) * Nx + sin(theta) * Ny);  // 在包围圆柱地面上的投影
}

Vector3 Bezier::F(double t, double u, double theta, Vector3 ray_O, Vector3 ray_V) {
    return S(u, theta) - C(t, ray_O, ray_V);
}

Eigen::Matrix3d Bezier::dF(double t, double u, double theta, Vector3 ray_V) {
    Vector3 dF_t = -ray_V;
    Vector3 dF_u = dP(u, Z) * (O2 - O1) +
                   dP(u, R) * (cos(theta) * Nx + sin(theta) * Ny);
    Vector3 dF_theta = P(u, R) * (-sin(theta) * Nx + cos(theta) * Ny);

    Eigen::Matrix3d matrix(3, 3);
    // 以列向量的方式输入矩阵中
    matrix << dF_t.ToEigenLibForm(), dF_u.ToEigenLibForm(), dF_theta.ToEigenLibForm();
    return matrix;
}

