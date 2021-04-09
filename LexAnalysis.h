#pragma once
#include<iostream>
#include<set>
#include<map>
using namespace std;

class FANode
{
public:
	char name;//状态结点名称
	FANode() {};
	FANode(char n) { name = n; }
};

bool operator==(const FANode& a, const FANode& b);

bool operator!=(const FANode& a, const FANode& b);

bool operator<(const FANode& a, const FANode& b);

class NFA
{
private:
	set<FANode> startSet;//初始状态集
	set<FANode> endSet;//终止状态集
	set<FANode>states;//状态集合
	string a;//输入符号集
	map<FANode, map<char, set<FANode>>> NFAtran;//NFA转换函数

public:
	//构造函数
	NFA() {};
	NFA(set<FANode>start,set<FANode>end,set<FANode>s,map<FANode,map<char,set<FANode>>>f){
		startSet = start;
		endSet = end;
		states = s;
		NFAtran = f;
	}

	void createNFA();//由正则表达式转换为NFA
	set<FANode> eClosure(set<FANode> T); //求从T中的某个状态开始只通过epsilon转换到达的状态集合
	set<FANode> move(set<FANode> T, char a); //求从T中某个状态s出发通过标号为a的转换到达的状态的集合


};

class DFA
{
private:
	FANode startSet;//初始状态
	set<FANode> endSet;//终止状态集
	set<FANode>states;//状态集合
	string a;//输入符号集
	map<FANode, map<char, FANode>> DFAtran;//DFA转换函数
public:
	//构造函数
	DFA() {};
	DFA(FANode start, set<FANode>end, set<FANode>s, map<FANode, map<char, FANode>>f) {
		startSet = start;
		endSet = end;
		states = s;
		DFAtran = f;
	}

	void subset(NFA& dfa); //用子集法将NFA确定化为DFA
	void minimum();//DFA的最小化


};