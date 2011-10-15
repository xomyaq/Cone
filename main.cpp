#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

double density0; // ��������� �����
double strength0; // �������������
double friction0; // ������ �����
double weight; // ����� ��������
double initialSpeed; // ��������� �������� ��������

double density(double); // ��������� ����� � ����������� �� ������� ����������
double strength(double); // �������������
double friction(double); // ������ ����� � ����������� �� ������� ����������
double f(double); // ��������� ����������� ��������
double df(double); // ����������� ��������� �����������
double k(double); // ������������ �����������
double F1(double); // ��������������� �������
double F2(double); // ��������������� �������
double f1(double); // ��������������� �������
double f2(double); // ��������������� �������
double force(double, double); // ����
double t(double, double); // �����
double integrate(double (*)(double), double, double); // ���������� ������ ��������
double g(double, double); // ������ ����� ����. ���������
double euler(double (*)(double, double), double, double, double); // ���������� ������ ������
double solve(ofstream&); // ������� ����. ��������� ������������� ��������

int main(int argc, char **argv) {
	double conicalIndenterWeight = 0.001 / 981;
	double conicalIndenterInitialSpeed = 30000;
	double solidHalfSpaceDensity = 0.0027 / 981;
	double solidHalfSpaceFriction = 0; 
//	double solidHalfSpaceFriction = 200; 
	double solidHalfSpaceStrength = 1000;

	density0 = solidHalfSpaceDensity;
	strength0 = solidHalfSpaceStrength;
	friction0 = solidHalfSpaceFriction;
	weight = conicalIndenterWeight;
	initialSpeed = conicalIndenterInitialSpeed;
	
	ofstream file;
    file.open ("spaceIndenter.dat");
    file.precision(6);
    file.setf(ios::fixed | ios::showpoint);
    cout.precision(6);
    cout.setf(ios::fixed | ios::showpoint);

	cout << "Working" << endl;
	solve(file);

	file.close();
//	system("pause");

	return 0;
}

double density(double z) {
	return density0;
}

double strength(double z) {
	return strength0;
}

double friction(double z) {
	return friction0;
}

double f(double z) {
	return 1 * z;
}

double df(double z) {
	double h = 0.0001;

	return (f(z + h) - f(z - h)) / (2 * h);
}

double k(double z) {
	return pow(sin(atan(df(z))), 2);
}

double F1(double x) {
	return k(x) * df(x) * f(x);
}

double F2(double x) {
	return (strength0 * df(x) + friction0) * f(x);
}

double f1(double x) {
	return df(x) * f(x);
}

double f2(double x) {
	return f(x);
}

double force(double x, double y) {
	return 2 * M_PI * ((strength0 + k(x) * density(x) * y) * integrate(f1, 0, x) + friction(x) * integrate(f2, 0, x));
}

double t(double t0, double dx, double v) {
	return t0 + dx / v;
}

double integrate(double (*func)(double), double a, double b) {
	double dx, x;
	double s = 0.0;
	int n = 1000;

	dx = (b - a) / n;
	for (int i = 2; i <= n - 1; i = i + 2) {
		x = a + i * dx;
		s = s + 2.0 * func(x) + 4.0 * func(x + dx);
    }
	s = (s + func(a) + func(b) + 4.0 * func(a + dx) ) * dx / 3.0;
	
	return s;
}

double g(double x, double y) {
	return -(4 * M_PI / weight) * density0 * integrate(&F1, 0, x) * y - (4 * M_PI / weight) * integrate(&F2, 0, x);
}

double test(double x, double y) {
	return -1 * y - 1;
}

double euler(double (*eq)(double, double), double x0, double y0, double xf) {
	double yf;
	
	yf = y0 + eq(x0, y0) * (xf - x0);
	yf = y0 + (eq(x0, y0) + eq(xf, yf)) * 0.5 * (xf - x0);
	
	return yf;
}

double solve(ofstream &fout) {
	double xk, yk, tk;
	double x0 = 0.0;
	double t0 = 0.0;
	double y0 = pow(initialSpeed, 2);
	double dx = 0.0001;
	double ymin = 0.0;

	while (y0 > ymin) {
				xk = x0 + dx;
		if (euler(test, x0, y0, xk) >= 0) {
			yk = euler(test, x0, y0, xk);
		} else {
			break;
		}
		tk = t(t0, dx, sqrt(yk));

		fout << setw(12) << xk << setw(18) << yk << endl;
		x0 = xk;
		y0 = yk;
		t0 = tk;
	}
//	while (y0 > ymin) {
//		xk = x0 + dx;
//		if (euler(g, x0, y0, xk) >= 0) {
//			yk = euler(g, x0, y0, xk);
//		} else {
//			break;
//		}
//		tk = t(t0, dx, sqrt(yk));
//
//		fout << setw(12) << xk << setw(18) << sqrt(yk) << setw(18) << force(xk, yk) << endl;
//		x0 = xk;
//		y0 = yk;
//		t0 = tk;
//	}

	return yk;
}