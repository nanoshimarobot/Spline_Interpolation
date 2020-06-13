#include <iostream>
#include <vector>
#include "Eigen/Core"
#include "Eigen/Geometry"
//#include "matplotlibcpp.h"

//namespace plt = matplotlibcpp;

std::vector<float> _a;
std::vector<float> _b;
std::vector<float> _c;
std::vector<float> _d;

void CalcParameter(const std::vector<float>& tx, const std::vector<float>& ty) {
	int TotalSection = ty.size() - 1;

	for (int i = 0; i <= TotalSection; i++) {
		_a.push_back(ty[i]);
	}

	Eigen::MatrixXf A = Eigen::MatrixXf::Zero(TotalSection + 1, TotalSection + 1);


	for (int i = 0; i <= TotalSection; i++) {
		if (i == 0 || i == TotalSection) {
			A(i, i) = 1;
		}
		else {
			float Hn1 = tx[i] - tx[i - 1];
			float Hn2 = tx[i + 1] - tx[i];
			A(i, i - 1) = Hn1;
			A(i, i) = 2.0f * (Hn1 + Hn2);
			A(i, i + 1) = Hn2;
		}
	}

	Eigen::MatrixXf B = Eigen::MatrixXf::Zero(TotalSection + 1, 1);
	for (int i = 0; i <= TotalSection; i++) {
		if (i != 0 && i != TotalSection) {
			float Hn1 = tx[i + 1] - tx[i];
			float Hn2 = tx[i] - tx[i - 1];
			B(i, 0) = ((3.0f / Hn1) * (_a[i + 1] - _a[i])) - ((3.0f / Hn2) * (_a[i] - _a[i - 1]));
		}
	}

	A = A.inverse();
	A = A * B;

	for (int i = 0; i < _a.size(); i++) {
		_c.push_back(A(i, 0));
	}

	for (int i = 0; i < TotalSection; i++) {
		float H = tx[i + 1] - tx[i];
		//float n = ((_a[i + 1] - _a[i]) / H) - (((_c[i + 1] + 2.0f * _c[i]) * H) / 3.0f);
		_b.push_back(((_a[i + 1] - _a[i]) / H) - (((_c[i + 1] + 2.0f * _c[i]) * H) / 3.0f));
	}

	for (int i = 0; i < TotalSection; i++) {
		float H = tx[i + 1] - tx[i];
		_d.push_back((_c[i + 1] - _c[i]) / (3.0f * H));
	}


	/*for (int i = 0; i < TotalSection; i++) {
		std::cout << "[" << i << "]" << ":";
		std::cout << "a:" << _a[i];
		std::cout << "b:" << _b[i];
		std::cout << "c:" << _c[i];
		std::cout << "d:" << _d[i] << std::endl;

	}*/
	/*std::cout << A << std::endl;
	std::cout << B << std::endl;
	A = A.inverse();
	A = A * B;
	std::cout << A << std::endl;*/

}

int CalcCurrentSection(const std::vector<float>& tx, float x) {
	int i = 0;
	while (1) {
		if (i == (tx.size() - 1)) {
			break;
		}
		if (tx[i] <= x && tx[i + 1] > x) {
			break;
		}
		i++;
	}
	return i;
}


float Interpolation(const std::vector<float>& tx, float x) {
	int i = CalcCurrentSection(tx, x);
	float dx = x - tx[i];
	return _a[i] + (_b[i] + (_c[i] + _d[i] * dx) * dx) * dx;
}

/*float CalcCurrentSection(const std::vector<float>& tx) {

}*/

int main() {
	FILE* gp;
	gp = _popen("gnuplot", "w");
	fprintf(gp, "unset key\n");
	fprintf(gp, "set xrange[0:10]\n");
	fprintf(gp, "set yrange[0:10]\n");
	fprintf(gp, "plot \"-\" with points \n");
	std::vector<float> tx{ 0.0f, 1.0f, 4.0f, 5.0f, 8.0f };
	std::vector<float> ty{ 0.0f, 3.0f, 4.0f, 1.0f, 2.0f };
	std::vector<float> x;
	std::vector<float> y;

	CalcParameter(tx, ty);

	for (float i = 0.0f; i <= tx[tx.size() - 1]; i += 0.01f) {
		/*x.push_back(i);
		y.push_back(Interpolation(tx, i));*/
		fprintf(gp, "%f, %f\n", i, Interpolation(tx, i));
	}

	fprintf(gp, "e\n");
	while (1) {
		fflush(gp);
	}
	//plt::plot(x, y, "--r");
	//plt::show();
	//return 0;
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
