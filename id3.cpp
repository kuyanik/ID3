#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

bool not_equal(string str, vector<string> str_arr){
	int i = 0;
	while(i < str_arr.size()){
		if(str == str_arr[i])
			return false;
		++i;
	}
	return true;
}

int count_att(string str){
	int i = 0;
	int count = 0;
	if(str.size() == 0)
		throw -1;
	while(i < str.size()){
		if(str[i] == ','){++count;}
		++i;
	}
	return count;
}

struct obj{

	vector<double> att;
	string type;

	obj(vector<double> &att, string &type){
		this->att = att;
		this->type = type;
	}

	void print(){
		int i = 0;
		while(i < att.size()){
			cout<<att[i]<<',';
			++i;
		}
		cout<<type<<endl;
	}
};

vector<string> find_types(vector<obj> arr_obj){

	vector<string> unique;
	unique.push_back(arr_obj[0].type);

	int i = 0;
	while(i < arr_obj.size()){
		if(arr_obj[0].type != arr_obj[i].type){
			unique.push_back(arr_obj[i].type);
		}
		++i;
	}
	return unique;
}

void extract(vector<obj> &arr_obj, string &str, unsigned int &count_comma){

	string temp_s;
	vector<double> temp_a;

	int i = 0;
	for(int j = 0;j < count_comma;++j){
		string temp;
		while(str [i]!= ','){
			temp.push_back(str[i]);
			++i;
		}
		temp_a.push_back(stod(temp));
		++i;
	}
	temp_s = str.substr(i, (str.size()-i+1));

	arr_obj.push_back(obj(temp_a, temp_s));

}

struct node{
	vector <obj> arr_obj;
	double cutoff,att_cutoff;
	node* left;
	node* right;

	node (vector<obj> arr_obj){
		this->arr_obj = arr_obj;
		left = nullptr;
		right = nullptr;
		cutoff = -5;
		att_cutoff = -5;
	}

	double entropy(vector<obj> bubble){
		vector<string> unique_types = find_types(bubble);
		if(unique_types.size() == 1){
			return 99;
		}

		int i = 0;
		double no = 0,yes = 0;
		while(i < bubble.size()){
			if(bubble[i].type == unique_types[0])
				++no;
			else if(bubble[i].type == unique_types[1])
				++yes;
			++i;
		}
		double p_no = no/(no+yes);
		double p_yes = yes/(no+yes);
		double entropy = -(p_no*log2(p_no))-(p_yes*log2(p_yes));

		return entropy;
	}

	void find_gain(){
		double gain = 0;
		double cutoff = 0;
		vector<obj> smaller;
		vector<obj> greater;
		int i = 0;
		while(i < arr_obj[0].att.size()){
			int j = 0;
			while(j < arr_obj.size()){
				cout<<j<<endl;
				cutoff = arr_obj[j].att[i];
				int k = 0;
				while(k < arr_obj.size()){
					if(arr_obj[k].att[i] <= cutoff){
						smaller.push_back(arr_obj[k]);
					}
					else if(arr_obj[k].att[i] > cutoff){
						greater.push_back(arr_obj[k]);
					}
					++k;
				}
				double pe1 = entropy(arr_obj);
				double pe2 = entropy(smaller);
				double pe3 = entropy(greater);
				if(pe1 == 99 || pe2 == 99 || pe3 == 99){ return;}
				double size_small = smaller.size();
				double size_great = greater.size();
				double size_all = arr_obj.size();

				double temp_gain = pe1 - ((size_small/size_all)*pe2) - ((size_great/size_all)*pe3);
				cout<<temp_gain<<endl;
				if(temp_gain > gain){
					this-> cutoff = cutoff;
					gain = temp_gain;
					this-> att_cutoff = i;
				}
				smaller.clear();
				greater.clear();
				++j;
			}
			++i;
		}
	}

};

int main(int argc,char **argv){
	ifstream file(argv[1], ifstream::in);

	unsigned int comma_count = 0;
	vector<obj> arr_obj;

	if(!file.is_open()){
		printf("Could not open the file\n");
	}
	else{
		string line;
		bool done = false;
		while(getline(file,line)){
			if(!done){
				comma_count = count_att(line);
				cout<<"Count of commas: "<<comma_count<<endl;
				done = true;
			}
				extract(arr_obj, line, comma_count);// very important, extracts the data from the file into objects.
		}
	}
	cout<<"OBJECT LENGTH"<<arr_obj.size()<<endl;

	int i = 0;
	while(i < arr_obj.size()){
		arr_obj[i].print();
		++i;
	}

	node* root = new node(arr_obj);
	root->find_gain();
	cout<<"CUTOFF "<< root->cutoff <<"ATTRIBUTE AT CUTOFF "<< root->att_cutoff <<endl;
	return 0;
}
