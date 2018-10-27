// ConsoleApplication7.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"

#include <any>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <cstring>
#include <fstream>
#include <iomanip>
class simplex {
private:
	bool s;
	bool flag;
	bool flag_optimal;
	bool decision;
	bool max_or_min;
	std::pair<int, int> element;
	std::vector<int> c_;
	std::vector<int> b_;
	std::vector<int> bp;
	std::vector<int> cp;
	std::vector<std::vector<double>> elements;
	std::vector<std::vector<double>> simplex_tabl;
	int rows;
	int columns;
	double perm;
public:
	simplex() {
		rows = 0;
		columns = 0;
		perm = 0;
		flag = true;
		flag_optimal = true;
		decision = false;
		max_or_min = true;
		s = false;
	};

	void algorithm(const std::string& str) {
		parse_file(str);
		create_simplex_tabl();
		write(std::cout);
		check_decision();
		optimal();
		inv();
		write(std::cout);
		std::cout << "F=" << simplex_tabl[rows][0];
	}
	void check_decision() {
		bool not_minus = true;
		for (int i = 0; i < rows && not_minus; ++i) {
			if (simplex_tabl[i][0] < 0) {
				for (int j = 1; j < columns + 1; ++j) {
					if (simplex_tabl[i][j] > 0) {
						flag = false;

					}
					else {
						flag = true;
						decision = true;
						not_minus = false;
						element.first = i;
						element.second = j;
						break;
					}
				}
			}
			else continue;
		}
	}
	void create_simplex_tabl() {
		if (s) {
			for (int i = 0; i < rows + 1; ++i) {
				for (int j = 0; j < columns + 1; ++j) {
					elements[i][j] *= -1;
				}
			}
	}
		simplex_tabl.resize(rows + 1);
		for (int i = 0; i < rows + 1; ++i) {
			simplex_tabl[i].resize(columns + 1);
				for (int j = 0; j < columns + 1; ++j) {
					simplex_tabl[i][j] = elements[i][j];
				}
		}
		bp.resize(rows);
		cp.resize(columns);
		for (int i = 0; i < columns; ++i) {
			cp[i] = (i + 1);
		}
		int k = 0;
		for (int i = 0; i < rows; ++i) {
			bp[i] = cp[columns - 1] + 1 + i;
		}
	}
	void optimal() {
		if (flag_optimal) {
			for (int i = 0; i < rows + 1; ++i) {
				for (int j = 0; j < columns + 1; ++j) {
					elements[i][j] = simplex_tabl[i][j];
				}
			}
			if (flag) {
//разрещающий элемент;
if (decision) decision = false;
else  element = rows_and_columns(element);
//заменив базис поменяв переменные местами

change_bazis(element);

for (int i = 1; i < columns + 1; ++i) {
	if (simplex_tabl[rows][i] > 0) {
		for (int j = 0; j < rows; ++j) {
			if (simplex_tabl[j][i] > 0) {
				flag_optimal = true;
				break;
			}
			else flag_optimal = false;
		}
		break;
	}
	else {
		flag_optimal = false;
	}
	if (!(flag_optimal)) {
		for (int i = 0; i < rows; ++i) {
			if (simplex_tabl[i][0] < 0) {
				flag_optimal = true;
				break;
			}
		}
	}
}
write(std::cout);
optimal();
		}
			else std::cout << "there is no decision";
	}
		else std::cout << " Optimal decision";
	}
	void change_bazis(std::pair<int, int>& element) {
		//изменение столбца и строки
		int b = element.second - 1;
		int c;
		if (bp.size() == rows && element.first == rows) {
			c = element.first - 1;
		}
		else c = element.first;
		std::swap(cp[b], bp[c]);
		for (int i = 0; i < rows + 1; ++i) {
			if (i == element.first) {
				simplex_tabl[i][element.second] = 1 / elements[element.first][element.second];
			}
			else  simplex_tabl[i][element.second] /= -elements[element.first][element.second];
		}

		for (int i = 0; i < columns + 1; ++i) {
			if (i == element.second);
			else simplex_tabl[element.first][i] /= elements[element.first][element.second];
		}

		for (int i = 0; i < rows + 1; ++i) {
			for (int j = 0; j < columns + 1; ++j) {
				if (i != element.first && j != element.second) {
					simplex_tabl[i][j] = elements[i][j] - ((elements[i][element.second] * elements[element.first][j]) / (elements[element.first][element.second]));
				}
			}
		}

	}
	std::pair<int, int>& rows_and_columns(std::pair<int, int>& element) {
		//находим наибольший столбец
		int k = 1;

		for (int j = 1; j < columns; ++j) {
			if (simplex_tabl[rows][k] < simplex_tabl[rows][j + 1]) k = j + 1;
		}


		element.second = k;
		bool check = false;
		int m = 0;
		int q = 0;
		for (int i = 0; i < rows; ++i) {
			/*if (simplex_tabl[i][0] > 0 && simplex_tabl[i + 1][0] < 0) {
				for (int j = 1; j < columns; ++j) {
					if ((simplex_tabl[i + 1][0] / simplex_tabl[i + 1][j]) > 0) {
						if ((simplex_tabl[i + 1][0] / simplex_tabl[i + 1][j]) > (simplex_tabl[i + 1][0] / simplex_tabl[i + 1][j + 1])) {
							if ((simplex_tabl[i + 1][0] / simplex_tabl[i + 1][j + 1]) > 0) {
								q = i + 1;
							}
						}
					}
				}
			}*/
			for (int k = 0; k < rows; ++k) {
				if (simplex_tabl[k][0] < 0) {
					m = k;
					check = true;
					break;
				}
			}
			if (check) {
				for (int j = 1; j < columns; ++j) {
					if ((simplex_tabl[m][0] / simplex_tabl[m][j]) > 0) {
						if ((simplex_tabl[m][0] / simplex_tabl[m][j]) > (simplex_tabl[m][0] / simplex_tabl[m][j + 1])){
							if ((simplex_tabl[m][0] / simplex_tabl[m][j + 1]) > 0) {
								q = m;
								element.second = j + 1;
						}
}
					}
				}
				break;
			}
			else if ((simplex_tabl[i][0] / simplex_tabl[q][element.second]) > (simplex_tabl[i + 1][0] / simplex_tabl[i + 1][element.second])) {
				if ((simplex_tabl[i + 1][0] / simplex_tabl[i + 1][element.second]) > 0) {
					q = i + 1;
				}
			
			}
			else if ((simplex_tabl[i][0] / simplex_tabl[q][element.second]) < 0) {
				if ((simplex_tabl[i + 1][0] / simplex_tabl[i + 1][element.second]) > 0) {
					q = i + 1;
				}
			}
			
		}
		element.first = q;
		return element;
	}
	std::ostream& write(std::ostream& stream) {
		
			stream << std::endl;
			stream << std::setw(14) << "S";
			for (int i = 0; i < columns; ++i) {
				stream << std::setw(9) << "X" << cp[i];
			
			}
			stream << std::endl;
			for (int i = 0; i < rows + 1; ++i) {
				if (i < rows) stream << std::setw(3) << "X" << bp[i];
				else stream << std::setw(4) << "F";
				for (int j = 0; j < columns + 1; ++j) {
					if (simplex_tabl[i][j] == int(simplex_tabl[i][j])) {

						stream << std::setw(10) << simplex_tabl[i][j];

					}
					else {
						stream << std::setw(10) << std::setprecision(3) << simplex_tabl[i][j];
					}
					
				}
				stream << std::endl << std::endl;
			}
			return stream;
	
	}
	void parse_file(const std::string& str) {
		char op;
		std::ifstream stream;
		stream.open(str.c_str());
		if (stream.is_open()) {
			if (stream >> op && op == 'c' && stream >> op && op == '=') {
				std::string end;
				//bool flag = true;
				int Num;
				stream >> op;
				if (op == '(') {
					
					while (stream >> op) {
						if (op == ',') {
							std::stringstream str_ch(end);
							str_ch >> Num;
							c_.push_back(Num);
							++columns;
							end.clear();
						}
						else if (op == ')') {
							std::stringstream str_ch(end);
							str_ch >> Num;
							c_.push_back(Num);
							++columns;
							end.clear();
							break;
							
						}
						else end.push_back(op);
					}
			
				
					
					
				}
			}

			if (stream >> op && op == 'b' && stream >> op && op == '=') {
				bool flag = true;
				std::string end;
				//bool flag = true;
				int Num;
				stream >> op;
				if (op == '(') {
					while (stream >> op) {
						if (op == ',') {
							std::stringstream str_ch(end);
							str_ch >> Num;
							b_.push_back(Num);
							++rows;
							end.clear();
						}
						else if (op == ')') {
							std::stringstream str_ch(end);
							str_ch >> Num;
							b_.push_back(Num);
							++rows;
							end.clear();
							break;

						}
						else end.push_back(op);
					}
				}
			}
			if (s) std::swap(rows, columns);
			if (stream >> op && op == 'a' && stream >> op && op == '=') {
				double Num;
				int k = 0, l = 0;
				stream >> Num;
				elements.resize(rows+1);
				if (s) {
					for (int i = 0; i < columns + 1; ++i) {
						for (int j = 0; j < rows + 1; ++j) {
							if (i == 0) {
								elements[j].resize(columns + 1);
								if (j == rows) {
									elements[j][i] = 0;
								}
								else { 
									elements[j][i] = c_[k];
									++k;
								}
							}
							else if (j == rows && i > 0) {
								elements[j][i] = b_[l];
								++l;
							}
							else {
								elements[j][i] = Num;
								stream >> Num;
							}
						}
					}
				}
				else {
					for (int i = 0; i < rows + 1; ++i) {
						elements[i].resize(columns + 1);
						for (int j = 0; j < columns + 1; ++j) {
							if (j == 0) {
								if (i == rows) {
									elements[i][j] = 0;
								}
								else {

									elements[i][j] = b_[k];
									++k;
								}

							}
							else if (j > 0 && i == rows) {
								elements[i][j] = c_[l];
								++l;
							}
							else {
								elements[i][j] = Num;
								stream >> Num;
							}

						}
					}
				}
				
			}
		
			stream.clear();
		}
	}
	void inv() {
		if (!(max_or_min)) simplex_tabl[rows][0] *= -1;
	}
	void max_or_min1(bool& q) {
		if (!(q)) max_or_min = false;
	}
	void S_or_L(bool& q) {
		if (q) s = true;
		else false;
	}
	~simplex() {}
};
int main()
{
	simplex S;
	char op;
	std::string str;
	std::string m_or_m, s_or_l;
	std::cout << "Cin path file" << std::endl;
	std::cin >> str;
	std::cout << "S or L" << std::endl;
	std::cin >> s_or_l;
	bool Si = false;
	if (s_or_l == "S") Si = true;
	S.S_or_L(Si);
	std::cout << " max or min?" << std::endl;
	std::cin >> m_or_m;
	bool m_m = true;

	std::istringstream stream(m_or_m);
	std::string str2;
	while (stream >> op) {
		str2.push_back(op);
	}
	if (str2 == "min") m_m = true;
	else if (str2 == "max") m_m = false;

	S.max_or_min1(m_m);
	S.algorithm(str);

	
	
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
