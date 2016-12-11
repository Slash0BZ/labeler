#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
using namespace std;

vector<string> split_by_space(string in){
	int base = 0;
	vector<string> ret;
	for(int i = 0; i < in.length(); i++){
		if(in[i] == ' '){
			ret.push_back(in.substr(base, i - base));	
			base = i + 1;
		}
	}
	ret.push_back(in.substr(base, in.size() - base));
	return ret;
}
int build_database(char* filename, map<int, map<string, int> > &freq_map, map<int, int> &count_map, vector<string> &testinfo, int &total_sample, int &total_test){
	ifstream input;
	input.open(filename);
	if (!input.is_open()){
		cout << "error: in reading input file \"" << filename << "\"" << endl;
		return 0;
	}
	string info;
	getline(input, info);
	vector<string> infovec = split_by_space(info);
	total_sample = stoi(infovec[0]);
	total_test = stoi(infovec[1]);
	cout << total_test << endl;
	for(int i = 0; i < total_sample; i++){
		string label;
		string question;
		getline(input, label);
		getline(input, question);

		vector<string> label_vec = split_by_space(label);
		vector<string> question_vec = split_by_space(question);	

		for(int j = 0; j < label_vec.size(); j++){
			int label_index = stoi(label_vec[j]);
			if (count_map.find(label_index) != count_map.end()){
				count_map[label_index] += 1;
			}
			else{
				count_map[label_index] = 1;
			}
			for(int k = 0; k < question_vec.size(); k++){
				if (freq_map[label_index].find(question_vec[k]) != freq_map[label_index].end()){
					freq_map[label_index][question_vec[k]] += 1;
				}
				else{
					freq_map[label_index][question_vec[k]] = 1;
				}
			}
		}
	}
	for(int i = 0; i < total_test; i++){
		string curtest;	
		getline(input, curtest);
		testinfo.push_back(curtest);
	}
	return 1;
}
int main(int argc, char** argv){
	if (argc < 3){
		cout << "usage: ./nb [input_file] [output_file]" << endl;
		return 1;
	}
	char* input_file_name = *(argv + 1);
	char* output_file_name = *(argv + 2);
	
	map<int, map<string, int> > freq_map;
	map<int, int> count_map;
	vector<string> testinfo;
	int total_sample, total_test;
	if (!build_database(input_file_name, freq_map, count_map, testinfo, total_sample, total_test)){
		return 1;
	}
	double bias = (double)count_map.size();
	for(int i = 0; i < total_test; i++){
		vector<string> curvec = split_by_space(testinfo[i]);
		map<int, double> prob_map;
		for(auto it = count_map.begin(); it != count_map.end(); it++){
			int label = it->first;
			map<string, int> curfreq= freq_map[label];
			double bias_up = bias * (double)count_map[label] / (double)total_sample;	
			for(int j = 0; j < curvec.size(); j++){
				double prob_cur_word = (double)prob
			}
			
		}
	}


	return 0;
}
