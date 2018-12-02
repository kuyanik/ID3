#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

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

vector<string> find_types(vector<obj> arr_obj){ //finds type names: setosa,versicolor. Can be altered to find more than 2 types(via remember) but according to Karol we deal with 2 types max.
	if(arr_obj.size() == 0){		//returns null vector if array is size 0 or else returns 1 or 2 unique types.
		return vector<string>();
	}
	vector<string> unique;
	unique.push_back(arr_obj[0].type);
	int i = 0;
	while(i < arr_obj.size()){
		if(arr_obj[0].type != arr_obj[i].type){
			unique.push_back(arr_obj[i].type);
			break;
		}
		++i;
	}
	return unique;
}

struct node{
	vector <obj> arr_obj;
	vector<string> decision, unique_types;
	node* left;
	node* right;

	node (vector<obj> arr_obj,vector<string> unique_types, vector<string> decision = {"At root node"}){
		this->unique_types = unique_types;
		this->arr_obj = arr_obj;
		this->decision = decision;
		left = nullptr;
		right = nullptr;
		split();
	}
	vector<int> each_type_count(vector<obj> arr_obj){
		int type1 = 0,type2 = 0;
		int i = 0;
		while(i < arr_obj.size()){
			if(arr_obj[i].type == this->unique_types[0]){
				++type1;
			}
			else if(arr_obj[i].type == this->unique_types[1])
				++type2;
			++i;
		}
		return vector<int>{type1,type2};
	}

	double entropy(vector<obj> arr_obj){
		vector<int> type_count = each_type_count(arr_obj);
		if(type_count[0] == 0 || type_count[1] == 0){ //homogenous or empty
			return 0;
		}

		double p_no =  double(type_count[0])/double(( type_count[0]+type_count[1]));
		double p_yes =  double(type_count[1])/double(( type_count[0]+ type_count[1]));
		double entropy = -(p_no*log2(p_no))-(p_yes*log2(p_yes));
		return entropy;
	}

	vector<double> find_split_point(vector<obj> arr_obj){//find split point
		double gain = 0, cutoff = 0, att_cutoff = 0;
		vector<obj> smaller,greater;
		int i = 0;
		int j = 0;
		while(i < arr_obj[0].att.size()){
			j = 0;
 			while(j < arr_obj.size()){
				double temp_cutoff = arr_obj[j].att[i];
				int k = 0;
				while(k < arr_obj.size()){
					if(arr_obj[k].att[i] <= temp_cutoff){
						smaller.push_back(arr_obj[k]);
					}
					else if(arr_obj[k].att[i] > temp_cutoff){
						greater.push_back(arr_obj[k]);
					}
					++k;
				}
				double pe1 = entropy(arr_obj);
				double pe2 = entropy(smaller);
				double pe3 = entropy(greater);

				double size_smaller = smaller.size();
				double size_greater = greater.size();
				double size_all = arr_obj.size();

				double temp_gain = pe1 - ((size_smaller/size_all)*pe2) - ((size_greater/size_all)*pe3);
				if(temp_gain > gain){
					cutoff = temp_cutoff;
					att_cutoff = i;
					gain = temp_gain;
				}
				smaller.clear();
				greater.clear();
				++j;
			}
			++i;
		}
		cout<<gain<<endl;
		return vector<double>{cutoff,att_cutoff};
	}

	void split(){
		vector<int> type_count = each_type_count(this->arr_obj);
		if(type_count[0] == 0 || type_count[1] == 0){ //homogenous or empty
			return;
		}

		vector<double> split_point = find_split_point(this->arr_obj); // index 0 cutoff , index 1 attribute
		vector<obj> smaller,greater;
		int i = 0;
		while(i < arr_obj.size()){
			double cutoff = split_point[0];
			if(arr_obj[i].att[static_cast<int>(split_point[1])] <= split_point[0]){
				smaller.push_back(arr_obj[i]);
			}
			else if(arr_obj[i].att[static_cast<int>(split_point[1])] > split_point[0]){
				greater.push_back(arr_obj[i]);
			}
			++i;
		}
		string s_ltemp = ("Attribute_" + to_string( int(split_point[1]+1)) + " <= " + to_string(split_point[0]));
		string s_rtemp = ("Attribute_" + to_string( int(split_point[1]+1)) + " > " + to_string(split_point[0]));
		this->decision.push_back(s_ltemp);
		left = new node(smaller,this->unique_types, this->decision);
		this->decision.pop_back();
		this->decision.push_back(s_rtemp);
		right = new node(greater,this->unique_types, this->decision);
		this->decision.pop_back();
	}

	void node_print(){
		if(left == nullptr && right == nullptr){
			int i = 1;
			while(i < decision.size()){
				cout<<decision[i]<<endl;
				++i;
			}
			vector<int> type_count = each_type_count(arr_obj);
			cout<<"Number of total items: "<<arr_obj.size()<<endl;
			cout<<unique_types[0]<<" count is "<<type_count[0]<<" and "<<unique_types[1]<<" count is "<<type_count[1]<<endl;
			return;
		}

		left->node_print();
		right->node_print();
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
	cout<<"OBJECT LENGTH: "<<arr_obj.size()<<endl;
	node* root = new node(arr_obj, find_types(arr_obj));
	root->node_print();
	return 0;
}
