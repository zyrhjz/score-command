#include<iostream>
#include<vector>
#include<limits>
#include<string>
#include<sstream>
double avg(const std::vector<int>& scores) {
	int n = scores.size();
	int sum = 0;
	for (int i = 0;i < n;i++) {
		sum += scores[i];
	}
	double average = static_cast<double>(sum) / n;
	return average;
}
int maxscore(const std::vector<int>& scores) {
	int max = -1;
	for (int x : scores) {
		if (x > max) {
			max = x;
		}
	}
	return max;
}
int count_passed(const std::vector<int>& scores) {
	int count = 0;
	for (int x : scores) {
		if (x >= 60) {
			count++;
		}
	}
	return count;
}
int main() {
	std::vector<int> scores;
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
		std::istringstream tmp(choicetoken);
		tmp >> input;
		if (!tmp || !tmp.eof()) {
			std::cout << "请输入 0 到 3 的整数\n";
			continue;
		}
		if (input == 0) {
			break;
		}
		else if (input == 1) {
			scores.clear();
			while(scores.size()<3) {
				std::string token;
				int x;
				std::cin >> token;
				if (!std::cin) {
					return 0;
				}
				std::istringstream temp(token);
				temp >> x;
				if (!temp||!temp.eof()){
					std::cout << "请输入整数" << std::endl;
					continue;
				}
				if (x < 0 || x>100) {
					std::cout << "成绩必须在 0 到 100 之间\n";
					continue;
				}
				scores.push_back(x);
			}
		}
		else if (input == 2) {
			for (int x : scores) {
				std::cout << x << std::endl;
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