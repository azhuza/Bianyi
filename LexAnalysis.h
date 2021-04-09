#pragma once
#include<iostream>
#include<set>
#include<map>
using namespace std;

class FANode
{
public:
	char name;//״̬�������
	FANode() {};
	FANode(char n) { name = n; }
};

bool operator==(const FANode& a, const FANode& b);

bool operator!=(const FANode& a, const FANode& b);

bool operator<(const FANode& a, const FANode& b);

class NFA
{
private:
	set<FANode> startSet;//��ʼ״̬��
	set<FANode> endSet;//��ֹ״̬��
	set<FANode>states;//״̬����
	string a;//������ż�
	map<FANode, map<char, set<FANode>>> NFAtran;//NFAת������

public:
	//���캯��
	NFA() {};
	NFA(set<FANode>start,set<FANode>end,set<FANode>s,map<FANode,map<char,set<FANode>>>f){
		startSet = start;
		endSet = end;
		states = s;
		NFAtran = f;
	}

	void createNFA();//��������ʽת��ΪNFA
	set<FANode> eClosure(set<FANode> T); //���T�е�ĳ��״̬��ʼֻͨ��epsilonת�������״̬����
	set<FANode> move(set<FANode> T, char a); //���T��ĳ��״̬s����ͨ�����Ϊa��ת�������״̬�ļ���


};

class DFA
{
private:
	FANode startSet;//��ʼ״̬
	set<FANode> endSet;//��ֹ״̬��
	set<FANode>states;//״̬����
	string a;//������ż�
	map<FANode, map<char, FANode>> DFAtran;//DFAת������
public:
	//���캯��
	DFA() {};
	DFA(FANode start, set<FANode>end, set<FANode>s, map<FANode, map<char, FANode>>f) {
		startSet = start;
		endSet = end;
		states = s;
		DFAtran = f;
	}

	void subset(NFA& dfa); //���Ӽ�����NFAȷ����ΪDFA
	void minimum();//DFA����С��


};