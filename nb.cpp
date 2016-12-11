#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
using namespace std;
vector<string> split_by_space(string in){
	int base = 0;
	vector<string> ret;
	for(int i = 0; i < in.length(); i++){
		if(in[i] == ' '){
			ret.push_back(in.substr(base, i - base));	
			while (in[i + 1] == ' '){
				i++;
			}
			base = i + 1;
		}
	}
	ret.push_back(in.substr(base, in.size() - base));
	return ret;
}
void processQuestionString(string &q){
	for(int i = 0; i < q.length(); i++){
		//if (q[i] == '?' || q[i] == '\'' || q[i] == ',' || q[i] == '/'){
		if (q[i] == '\'' && q[i+1] == 's'){
			q[i] = ' ';
			q[i+1] = ' ';
		}
	}
	int lastNonSpace = q.length() - 1;
	for(; lastNonSpace >= 0; lastNonSpace--){
		if (q[lastNonSpace] != ' '){
			break;
		}
	}
	if (lastNonSpace != q.length() - 1){
		q.erase(lastNonSpace + 1, q.length() - lastNonSpace - 1);
	}
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
	for(int i = 0; i < total_sample; i++){
		string label;
		string question;
		getline(input, label);
		getline(input, question);

		processQuestionString(question);

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

	input.close();
	return 1;
}
bool pair_comparator(pair<int, double> a, pair<int, double> b){
	return a.second > b.second;
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
	ofstream output;
	output.open(output_file_name);
	double bias = (double)count_map.size();
	for(int i = 0; i < total_test; i++){
		vector<string> curvec = split_by_space(testinfo[i]);
		vector< pair<int, double> > prob_pair;
		for(auto it = count_map.begin(); it != count_map.end(); it++){
			int label = it->first;
			map<string, int> curfreq= freq_map[label];
			double labelCount = (double)count_map[label];
			double bias_up = bias * labelCount / (double)total_sample;	
			double wordProb = log(labelCount / (double)total_sample);
			for(int j = 0; j < curvec.size(); j++){
				auto pos = curfreq.find(curvec[j]);
				if (pos != curfreq.end()){
					double prob_cur_word = ((double)(pos->second) + 1) / (labelCount + bias + 1);
					wordProb += log(prob_cur_word);
				}
				else{
					double prob_cur_word = 1 / (labelCount + bias);
					wordProb += log(prob_cur_word);
				}
			}
			prob_pair.push_back(make_pair(label, wordProb));
		}
		sort(prob_pair.begin(), prob_pair.end(), pair_comparator);

		for(int oi = 0; oi < 10; oi++){
			//output << prob_pair[oi].first << ": " << prob_pair[oi].second << "; ";
			output << prob_pair[oi].first;
			if (oi != 4)
				output << " ";
		}
		output << "\n";
	
	}
	output.close();


	return 0;
}
