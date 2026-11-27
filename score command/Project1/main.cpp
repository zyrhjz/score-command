#define NOMINMAX
#include<iostream>
#include<vector>
#include<limits>
#include<string>
#include<sstream>
#include<stdexcept>
#include<mysql.h>
#include<Windows.h>
#include<memory>
class studentscore {
private:
	std::string studentid;
	std::string name;
	int score;
public:
	studentscore(const std::string& id, const std::string& sname, const int& x) :studentid(id),name(sname),score(x){
		if (x < 0 || x>100) {
			throw std::invalid_argument("成绩必须在0-100之间");
		}
	};
	int getscore() const{
		return score;
	}
	const std::string& getstudentid() const{
		return studentid;
	}
	const std::string& getname() const {
		return name;
	}
};
double avg(const std::vector<studentscore>& scores) {
	int n = scores.size();
	int sum = 0;
	for (int i = 0;i < n;i++) {
		sum += scores[i].getscore();
	}
	double average = static_cast<double>(sum) / n;
	return average;
}
int maxscore(const std::vector<studentscore>& scores) {
	int max = -1;
	for (const studentscore& record: scores) {
		if (record.getscore() > max) {
			max = record.getscore();
		}
	}
	return max;
}
int count_passed(const std::vector<studentscore>& scores) {
	int count = 0;
	for (const studentscore& record:scores) {
		if (record.getscore() >= 60) {
			count++;
		}
	}
	return count;
}
bool parseinteger(const std::string& x,int& value) {
	int result;
	std::istringstream tmp(x);
	tmp >> result;
	if (!tmp || !tmp.eof()) {
		return false;
	}
	else {
		value = result;
		return true;
	}
}





int main() {
	SetConsoleOutputCP(CP_UTF8);
	std::cout << "请输入mysql用户名" << std::endl;
	std::string dbuser;
	std::getline(std::cin,dbuser);
	std::cout << "请输入mysql密码" << std::endl;
	std::string dbpassword;
	std::getline(std::cin, dbpassword);
	MYSQL* connection = mysql_init(nullptr);
	std::unique_ptr<MYSQL, decltype(&mysql_close)> connectionOwner(connection, &mysql_close);
	if (connection == nullptr) {
		std::cout<< "初始化失败" << std::endl;
		return 1;
	}
	else {
		std::cout << "初始化成功" << std::endl;
		MYSQL* connected = mysql_real_connect(
			connection,
			"127.0.0.1",
			dbuser.c_str(),
			dbpassword.c_str(),
			"SCORE_COMMAND",
			3307,
			nullptr,
			0
		);
		if (connected == nullptr) {
			std::cout << "连接失败" << std::endl;
			std::cout << mysql_error(connection) << std::endl;
			return 1;
		}
		else {
			std::cout << "连接成功" << std::endl;
		}
	}
	std::vector<studentscore> scores;
	int input;
	std::string choicetoken;
	while (true) {
		std::cout << "1. 录入成绩" << std::endl;
		std::cout << "2.查看成绩" << std::endl;
		std::cout << "3.显示统计" << std::endl;
		std::cout << "4.按学号查询历史成绩" << std::endl;
		std::cout << "5.创建考试" << std::endl;
		std::cout << "0.退出" << std::endl;
		if (!(std::cin >> choicetoken)) {
			if (std::cin.eof()) {
				return 0;
			}
			else {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "请输入 0 到 5 的整数\n";
				continue;
			}
		}
		if(!parseinteger(choicetoken,input)){
			std::cout << "请输入 0 到 5 的整数\n";
			continue;
		}
		if (input == 0) {
			break;
		}
		else if (input == 1) {
			std::string input_pnum;
			int pnum;
			std::cout << "请输入录入人数(1-100)" << std::endl;
			while (true) {
				std::cin >> input_pnum;
				if (!std::cin) {
					return 0;
				}
				if (!parseinteger(input_pnum, pnum)) {
					std::cout << "请输入1-100之间的整数" << std::endl;
					continue;
				}
				if (pnum < 1 || pnum>100) {
					std::cout << "请输入1-100之间的整数" << std::endl;
					continue;
				}
				break;
			}
			scores.clear();
			while(scores.size()<pnum) {
				std::string token;
				std::string sid;
				std::string sname;
				int x;
				std::cin >> sid >> sname>>token;
				if (!std::cin) {
					return 0;
				}
				if (!parseinteger(token, x)) {
					std::cout << "请输入整数" << std::endl;
					continue;
				}
				if (x < 0 || x>100) {
					std::cout << "成绩必须在 0 到 100 之间" << std::endl;
					continue;
				}
				studentscore record(sid, sname, x);
				scores.push_back(record);
			}
		}
		else if (input == 2) {
			for (const studentscore& record :scores) {
				std::cout <<"学号:" << record.getstudentid() << std::endl;
				std::cout << "学生姓名:" << record.getname() << std::endl;
				std::cout << "成绩:" << record.getscore() << std::endl;
			}
		}
		else if (input == 3) {
			if (scores.empty()) {
				std::cout << "请先录入成绩" << std::endl;
			}
			else {
				std::cout << "平均分:" << avg(scores) << std::endl;
				std::cout << "最高分:" << maxscore(scores) << std::endl;
				std::cout << "及格人数:" << count_passed(scores) << std::endl;
			}
		}
		else if (input == 4) {
			std::string studentid;
			std::cout << "请输入学号" << std::endl;
			std::cin >> studentid;
			if (!std::cin) {
				return 0;
			}
			else {
				MYSQL_STMT* statement = mysql_stmt_init(connection);
				if (statement==nullptr) {
					std::cout << "创建查询语句失败" << std::endl;
					continue;
				}
				else {
					std::unique_ptr<MYSQL_STMT, decltype(&mysql_stmt_close)> statementowner(statement, &mysql_stmt_close);
					const std::string historysql =
						"SELECT E.EXAM_NAME,ES.SCORE "
						"FROM EXAM_SCORE AS ES "
						"JOIN STUDENT AS S "
						"ON ES.STUDENT_ID=S.STUDENT_ID "
						"JOIN EXAM AS E "
						"ON ES.EXAM_ID=E.EXAM_ID "
						"WHERE ES.STUDENT_ID=? "
						"ORDER BY E.EXAM_DATE,E.EXAM_ID ";
					int queryStatus = mysql_stmt_prepare(
						statement,
						historysql.c_str(),
						static_cast<unsigned long>(historysql.size())
					);
					if(queryStatus!=0) {
						std::cout << "准备查询失败" << std::endl;
						std::cout << mysql_stmt_error(statement) << std::endl;
						continue;
					}
					unsigned long studentidlength = static_cast<unsigned long>(studentid.size());
					MYSQL_BIND parameters[1]{};
					parameters[0].buffer_type = MYSQL_TYPE_STRING;
					parameters[0].buffer = studentid.data();
					parameters[0].buffer_length = studentidlength;
					parameters[0].length = &studentidlength;
					int bindstatus = mysql_stmt_bind_param(statement, parameters);
					if (bindstatus == 0) {
						int executestatus = mysql_stmt_execute(statement);
						if (executestatus == 0) {
							char examname[81]{};
							unsigned long examnamelength = 0;
							int examscsore = 0;
							MYSQL_BIND resultcolumns[2]{};
							resultcolumns[0].buffer_type = MYSQL_TYPE_STRING;
							resultcolumns[0].buffer_length = sizeof(examname);
							resultcolumns[0].length = &examnamelength;
							resultcolumns[0].buffer = examname;
							resultcolumns[1].buffer_type = MYSQL_TYPE_LONG;
							resultcolumns[1].buffer = &examscsore;
							int resultbindstatus = mysql_stmt_bind_result(statement, resultcolumns);
							if (resultbindstatus == 0) {
								int fetchstatus = mysql_stmt_fetch(statement);
								if (fetchstatus == MYSQL_NO_DATA) {
									std::cout << "未找到该学号的历史成绩" << std::endl;
									continue;
								}
								while (fetchstatus == 0) {
									std::cout << std::string(examname, examnamelength) << " " << examscsore << std::endl;
									fetchstatus = mysql_stmt_fetch(statement);
								}
								if (fetchstatus != MYSQL_NO_DATA) {
									std::cout << "读取历史成绩失败" << std::endl;
									std::cout << mysql_stmt_error(statement) << std::endl;
									continue;
								}
							}
							else {
								std::cout << "结果绑定失败" << std::endl;
								std::cout<<mysql_stmt_error(statement)<<std::endl;
								continue;
							}
						}
						else {
							std::cout<<mysql_stmt_error(statement)<<std::endl;
						}
					}
					else {
						std::cout << mysql_stmt_error(statement) << std::endl;
						continue;
					}
				}
			}
		}
		else if (input == 5) {
			std::string examname;
			std::string examdate;
			std::cout << "请输入考试名称和考试日期" << std::endl;
			std::cin >> std::ws;
			std::getline(std::cin,examname);
			std::cin >> examdate;
		}
		else {
			std::cout << "无效选项，请输入 0 到 5" << std::endl;
		}
	}
	return 0;
}