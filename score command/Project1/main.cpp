#include<iostream>
#include<vector>
#include<limits>
#include<string>
#include<sstream>
#include<stdexcept>
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
	std::vector<studentscore> scores;
	int input;
	std::string choicetoken;
	while (true) {
		std::cout << "1. 录入成绩" << std::endl;
		std::cout << "2.查看成绩" << std::endl;
		std::cout << "3.显示统计" << std::endl;
		std::cout << "0.退出" << std::endl;
		if (!(std::cin >> choicetoken)) {
			if (std::cin.eof()) {
				return 0;
			}
			else {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "请输入 0 到 3 的整数\n";
				continue;
			}
		}
		if(!parseinteger(choicetoken,input)){
			std::cout << "请输入 0 到 3 的整数\n";
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
		else {
			std::cout << "无效选项，请输入 0 到 3" << std::endl;
		}
	}
	return 0;
}