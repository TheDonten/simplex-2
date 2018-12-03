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
#include <cmath>
#include <fstream>
#include <iomanip>
#include <math.h>
class simplex {
private:
	
	bool s;
	
	bool max_or_min;
	

	std::vector<int> c_;
	std::vector<int> b_;
	std::vector<int> bp;
	std::vector<int> cp;
	std::vector<std::vector<double>> elements;
	std::vector<std::vector<double>> simplex_tabl;
	
	int rows;
	int columns;
	
	int F;
	int m;
	
	double perm;
	bool meh = true;
public:
	bool CLP = false;
	simplex() {
		rows = 0;
		columns = 0;
		perm = 0;
		
		max_or_min = true;
		s = false;
	};

	void algorithm(const std::string& str) {
		parse_file(str);
		//save();
		create_simplex_tabl(rows,simplex_tabl, elements, cp, bp);
		write(std::cout,rows,simplex_tabl,cp,bp);
		std::pair<int, int> elem;
		bool flag = check_decision(rows, simplex_tabl, elements, elem, cp, bp);
		optimal(flag, elem, rows, simplex_tabl, elements, cp, bp);
		write(std::cout, rows, simplex_tabl, cp, bp);
		
		//inv();		
		F = -simplex_tabl[rows][0];
		if (CLP) {
			
			std::vector<std::vector<double>> proverka;
			proverka.resize(rows + 1);
			for (int i = 0; i < rows + 1; ++i) {
				proverka[i].resize(columns + 1);
				for (int j = 0; j < columns + 1; ++j) {
					proverka[i][j] = simplex_tabl[i][j];
				}
			}
			int count = rows + columns;
			std::vector<int> cc = cp;
			std::vector<int> bb = bp;
			m = rows;
			
			cl(proverka,elements, count,rows,cc,bb);
						
		}
		
		std::cout << "F=" << simplex_tabl[rows][0];
	}
	
	void cl(std::vector<std::vector<double>> k, std::vector<std::vector<double>> el, int count, int row, std::vector<int> c, std::vector<int>  b) {
		
		
		double l;
		std::vector<double> func;
		std::vector<double> rev;
		func.resize(columns + 1);
		rev.resize(columns);
		for (int i = 0; i < row; ++i) {
			l = (round(1000 * k[i][0]) / 1000);
			if (l != int(l) && (b[i] <= m)) {
				for (int j = 0; j < columns; ++j) {
					rev[j] = k[i][j + 1];
				}
				for (int p = 0; p < columns + 1; ++p) {
					func[p] = k[row][p];
				}
				++row;
				++count;
				b.push_back(count);
				
				left(k,el, l,row,func,rev,c,b,count);
				right(k, el, l, row, func, rev, c, b, count);
				break;
			
			}
		}
		
	}
	void okrug(int row, std::vector<std::vector<double>> &si, std::vector<int>  b) {
		double k,l;
		for (int i = 0; i < row + 1; ++i) {
			k = floor(si[i][0]);
			l = floor(si[i][0]) + 1;
			if (k - si[i][0] > -0.05) {
				si[i][0] = k;
			}
			else if (l - si[i][0] < 0.05) {
				si[i][0] = l;
			}
		}
	}
	void right(std::vector<std::vector<double>> k, std::vector<std::vector<double>> el, double num, int row, std::vector<double> func, std::vector<double> rev, std::vector<int>  c, std::vector<int>  b, int count) {
		double n = ceil(num);
		std::cout << " RIGHT" << std::endl << std::endl << std::endl;

		k.resize(row + 1);
		for (int i = 0; i < row + 1; ++i) {
			k[row].resize(columns + 1);
			for (int j = 0; j < columns + 1; ++j) {
				if (i == row - 1) {
					if (j == 0) k[i][j] = num - n;
					else k[i][j] = rev[j - 1];
				}
				else if (i == row) {
					k[i][j] = func[j];
				}
			}
		}
		write(std::cout, row, k, c, b);
		meh = false;
		create_simplex_tabl(row, el, k, c, b);
		std::pair<int, int> elem;
		bool flag = check_decision(row, k, el, elem, c, b);
		if (!(flag)) {
			std::cout << "Null soluion" << std::endl;
			return;
		}
		optimal(flag, elem, row, k, el, c, b);
		write(std::cout, row, k, c, b);
		okrug(row, k, b);
		double l;
		bool check = true;
		for (int i = 0; i < row; ++i) {
			l = (round(1000 * k[i][0])) / 1000;
			if (l != int(l) && b[i] <= m) {
				check = false;
				break;
			}
			else check = true;

		}
		double mec;
		mec = -k[row][0];
		if (check) {
			if (mec > F) {
				std::cout << "No decision" << std::endl;
				return;
			}
			else {
				std::cout << "CLP" << std::endl;
				for (int i = 0; i < row; ++i) {
					if (b[i] <= m) {
						std::cout << "X" << b[i] << "=" << k[i][0] << "  ";
					}
				}
				write(std::cout, row, k, c, b);
				return;
			}
		}
		else
		{
			for (int i = 0; i < row; ++i) {
				if (b[i] <= m) {
					std::cout << "X" << b[i] << "=" << k[i][0] << "  ";
				}
			}
			cl(k, el, count, row, c, b);
		}

	}
	void left(std::vector<std::vector<double>> k, std::vector<std::vector<double>> el, double num, int row, std::vector<double> func, std::vector<double> rev, std::vector<int>  c, std::vector<int>  b, int count) {
		double n = floor(num);
		std::cout << " LEFT" << std::endl << std::endl << std::endl;
		
		k.resize(row + 1);
		for (int i = 0; i < row + 1; ++i) {
			k[row].resize(columns + 1);
			for (int j = 0; j < columns + 1; ++j) {
				if (i == row - 1) {
					if (j == 0) k[i][j] = n - num;
					else k[i][j] = -rev[j - 1];
				}
				else if (i == row) {
					k[i][j] = func[j];
				}
			}
		}
		write(std::cout, row, k, c, b);
		meh = false;
		create_simplex_tabl(row, el, k, c, b);
		std::pair<int, int> elem;
		bool flag = check_decision(row, k, el, elem, c, b);
		if (!(flag)) {
			std::cout << "Null soluion" << std::endl;
			return;
		}
		optimal(flag,elem,row,k,el,c,b);
		write(std::cout, row, k, c, b);
		okrug(row, k, b);
		double l;
		bool check = true;
		for (int i = 0; i < row; ++i) {
			l = (round(1000 * k[i][0])) / 1000;
			if (l != int(l) && b[i] <= m) {
				check = false;
				break;
			}
			else check = true;

		}
		double mec;
		mec = -k[row][0];
		if (check) {
			if (mec > F) {
				std::cout << "No decision" << std::endl;
				return;
			}
			else{
				std::cout << "CLP" << std::endl;
				for (int i = 0; i < row; ++i) {
					if (b[i] <= m) {
						std::cout << "X" << b[i] << "=" << k[i][0] << "  ";
					}
				}
				write(std::cout, row, k, c, b);
				return;
			}
		}
		else
		{
			for (int i = 0; i < row; ++i) {
				if (b[i] <= m) {
					std::cout << "X" << b[i] << "=" << k[i][0] << "  ";
				}
			}
			cl(k, el, count, row, c, b);
		}
		
	}


	    bool check_decision(int row, std::vector<std::vector<double>> &si, std::vector<std::vector<double>> &e, std::pair<int,int> el, std::vector<int> &c, std::vector<int> &b) {
		bool not_minus = true;
		//bool meh = true;
		
		std::cout << " OPOR" << std::endl << std::endl << std::endl;
		write(std::cout, row, si, c, b);
		e.resize(row + 1);
		for (int i = 0; i < row + 1; ++i) {
			e[i].resize(columns + 1);
			for (int j = 0; j < columns + 1; ++j) {
				e[i][j] = si[i][j];
			}
		}
		bool decision = true;
		bool flag = false;
		for (int i = 0; i < row && decision; ++i) {
			if (si[i][0] < 0) {
				for (int j = 1; j < columns + 1; ++j) {
					if (si[i][j] > 0) {
						decision = false;
						flag = false;
					}
					else {
						decision = false;
						flag = true;
						el.first = i;
						el.second = j;
						break;
					}
				}
			}
		}
		if (flag) {
			change_bazis(row, el, si, e, c, b);
			check_decision(row, si, e,el,c, b);

		}
		else {
			
			return decision;
		}
		
	}
	void create_simplex_tabl(int row,std::vector<std::vector<double>> &si, std::vector<std::vector<double>> &e, std::vector<int> &c, std::vector<int> &b) {
		if (s) {
			for (int i = 0; i < row + 1; ++i) {
				for (int j = 0; j < columns + 1; ++j) {
					e[i][j] *= -1;
				}
			}
		}
		si.resize(row + 1);
		for (int i = 0; i < row + 1; ++i) {
			si[i].resize(columns + 1);
			for (int j = 0; j < columns + 1; ++j) {
				si[i][j] = e[i][j];
			}
		}
		if (meh) {
			b.resize(row);
			c.resize(columns);
			for (int i = 0; i < columns; ++i) {
				c[i] = (i + 1);
			}
			int k = 0;
			for (int i = 0; i < row; ++i) {
				b[i] = cp[columns - 1] + 1 + i;
			}
		}
	}
	void optimal(bool flag_optimal,std::pair<int,int> element,int row, std::vector<std::vector<double>> &si, std::vector<std::vector<double>> &e, std::vector<int> &c, std::vector<int> &b) {
		if (flag_optimal) {
			e.resize(row + 1);
			for (int i = 0; i < row + 1; ++i) {
				e[i].resize(columns + 1);
				for (int j = 0; j < columns + 1; ++j) {
					e[i][j] = si[i][j];
				}
			}
			
			
				//разрещающий элемент;
				  element = rows_and_columns(element,row,si);

				//заменив базис поменяв переменные местами
				  if (element.second > 0) { change_bazis(row, element, si, e, c, b);
				//  okrug(row, si);
				  }
				  write(std::cout, row, si, c, b);
				  bool flag = true;
				  for (int i = 0; i < row; ++i) {
					  if (si[i][0] < 0) {
						  flag = check_decision(row, si, e, element, c, b);
						  break;
					  }
				  }
				
				  if (flag) {
					  for (int j = 1; j < columns + 1; ++j) {
						  if (si[row][j] < 0) flag_optimal = false;
						  else {
							  flag_optimal = true;
							  break;
						  }
					  }
					  write(std::cout, row, si, c, b);
					  optimal(flag_optimal, element, row, si, e, c, b);
				  }
				  else {
					  write(std::cout, row, si, c, b);
					  throw std::logic_error("NUll");
				  }
				
			
		}
		else std::cout << " Optimal decision" << std::endl;
	}
	
	void change_bazis(int row,std::pair<int, int>& element, std::vector<std::vector<double>> &si, std::vector<std::vector<double>> &e, std::vector<int> &c, std::vector<int> &b) {
		//изменение столбца и строки

		int m = element.second - 1;
		
		int l;
		if (bp.size() == row && element.first == row) {
			l = element.first - 1;
		}
		else l = element.first;
		std::swap(c[m], b[l]);
		for (int i = 0; i < row + 1; ++i) {
			if (i == element.first) {
				si[i][element.second] = 1 / e[element.first][element.second];
			}
			else  si[i][element.second] /= -e[element.first][element.second];
		}

		for (int i = 0; i < columns + 1; ++i) {
			if (i == element.second);
			else si[element.first][i] /= e[element.first][element.second];
		}
		//(round(1000 * k[i][0])) / 1000;
		for (int i = 0; i < row + 1; ++i) {
			for (int j = 0; j < columns + 1; ++j) {
				if (i != element.first && j != element.second) {
					si[i][j] = (round(100 * (e[i][j] - ((e[i][element.second] * e[element.first][j]) / (e[element.first][element.second]))))) / 100;
				}
			}
		}

	}
	std::pair<int, int>& rows_and_columns(std::pair<int, int>& element,int row, std::vector<std::vector<double>> &si) {
		//находим наибольший столбец
		
		for (int i = 1; i < columns + 1; ++i) {
			if (si[row][i] > 0) {
				element.second = i;
				break;
			}
		}
		bool chase;
		for (int i = 0; i < row; ++i) {
			if (si[i][element.second] < 0) {
				chase = true;
			}
			else {
				chase = false;
				break;
			}
		}
		if (chase) throw std::logic_error("Null");
	
		element.first;
		double mimal = 10000000000000;
		int k = 0;
		for (int i = 0; i < row; ++i)
		{
			if (si[i][element.second] > 0) 
			{
				double time = si[i][0] / si[i][element.second];
				if (time < mimal && time >= 0) 
				{
					mimal = time;
					k = i;
				}
			}
		}
		element.first = k;
		return element;
	}
	std::ostream& write(std::ostream& stream,int row, std::vector<std::vector<double>> &si, std::vector<int> &c, std::vector<int> & b) {

		stream << std::endl;
		stream << std::setw(14) << "S";
		for (int i = 0; i < columns; ++i) {
			stream << std::setw(9) << "X" << c[i];

		}
		stream << std::endl;
		for (int i = 0; i < row + 1; ++i) {
			if (i < row) stream << std::setw(3) << "X" << b[i];
			else stream << std::setw(4) << "F";
			for (int j = 0; j < columns + 1; ++j) {
				if (si[i][j] == int(si[i][j])) {

					stream << std::setw(10) << si[i][j];

				}
				else {
					stream << std::setw(10) << std::setprecision(3) << si[i][j];
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
				elements.resize(rows + 1);
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
	~simplex() {
		
	}
};
int main()
{
	simplex S;
	char op;
	std::string str;
	std::string m_or_m, s_or_l;
	std::cout << "Cin path file" << std::endl;
	std::cin >> str;
	std::cout << "S or L or CLP" << std::endl;
	std::cin >> s_or_l;
	bool Si = false;
	if (s_or_l == "S") Si = true;
	else if (s_or_l == "CLP") S.CLP = true;
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
