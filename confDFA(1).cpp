#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

class ConditionNode
{
private:
	ConditionNode* next;
public:
	//节点序号
	int index;
	//节点跃迁的条件
	char condition;
	ConditionNode(int, char);
	//获取下一个节点
	ConditionNode* getnext();
	//设置下一个节点
	void setNext(ConditionNode*);
};
ConditionNode::ConditionNode(int index, char condition)
{
	this->index = index;
	this->condition = condition;
	setNext(NULL);
}
ConditionNode* ConditionNode::getnext() {
	return next;
}
void ConditionNode::setNext(ConditionNode* next) {
	next = next;
}

class Nodetransform
{
private:
	//邻接表首节点地址
	ConditionNode* adjoinTable;
public:
	//节点序号
	int index;
	Nodetransform(int);
	//设置邻接表首节点
	void setConditionNode(ConditionNode*);
	//获取邻接表首节点
	ConditionNode* getConditionNode();
	//添加邻接点
	void addConditionNode(ConditionNode*);
};
Nodetransform::Nodetransform(int index)
{
	this->index = index;
	setConditionNode(NULL);
}

void Nodetransform::setConditionNode(ConditionNode* firstNode) {
	adjoinTable = firstNode;
}

ConditionNode* Nodetransform::getConditionNode() {
	return adjoinTable;
}

void Nodetransform::addConditionNode(ConditionNode* newNode) {
	newNode->setNext(adjoinTable);
	adjoinTable = newNode;
}


class DFANode :public Nodetransform
{
private:
	set<int> nfaSet;
public:
	DFANode(int index, set<int>);
	DFANode(int index);
	set<int> getNFASet();
};
DFANode::DFANode(int index, set<int> list) :Nodetransform(index)
{
	this->nfaSet = list;
}
DFANode::DFANode(int index) : Nodetransform(index) {

}
set<int> DFANode::getNFASet() {
	return nfaSet;
}





class NFA
{
private:
	map<int, Nodetransform*> nodeMap;
	int sIndex;
	int zIndex;
public:
	~NFA();
	bool createNFA();
	map<int, Nodetransform*> getNodeMap();
	int getSIndex();
	int getZIndex();
};
map<int, Nodetransform*> NFA::getNodeMap() {
	return nodeMap;
}
int NFA::getSIndex() {
	return sIndex;
}
int NFA::getZIndex() {
	return zIndex;
}


class DFA
{
private:
	map<int, DFANode*> nodeList;
	int sIndex;
	vector<int> zIndex;
	set<int> move(set<int>, char, NFA*);
	set<int> closure(set<int>, NFA*);

public:
	DFA(NFA*);
	~DFA();
	void createDFA(NFA*);
};
DFA::DFA(NFA* nfa)
{
	sIndex = 0;
	createDFA(nfa);
}


DFA::~DFA()
{
	map<int, DFANode*>::iterator iter;
	iter = nodeList.begin();
	while (iter != nodeList.end()) {
		ConditionNode* nowAdjoin = nodeList[iter->first]->getConditionNode();
		while (nowAdjoin != NULL)
		{
			nowAdjoin = nowAdjoin->getnext();
			delete(nowAdjoin);
		}
		delete(nodeList[iter->first]);
		iter++;
	}
}





void DFA::createDFA(NFA* nfa) {
	//记录当前节点表中的最大序号
	int num = 0;
	//获取nfa的初态，与dfa相同
	sIndex = nfa->getSIndex();
	//尚未被遍历的状态编号
	vector<int> noOrder;
	//获取nfa的节点表
	map<int, Nodetransform*> nfaNodeList = nfa->getNodeMap();
	//把初始状态添加到dfa结点表中
	set<int> sSet;
	sSet.insert(sIndex);
	set<int> startStatus = closure(sSet, nfa);
	nodeList.insert(pair<int, DFANode*>(num, new DFANode(sIndex, startStatus)));
	//如果集合包含终态，就把它插入终态表
	if (startStatus.count(nfa->getZIndex()) != 0)
		zIndex.push_back(num);
	//把初始状态添加到noOrder队列里
	noOrder.push_back(sIndex);
	//开始从初始状态进行创建DFA
	set<int> nowStatus;
	//存储每次新生成的集合
	set<int> newSet;
	while (!noOrder.empty()) {
		//从onOrder队列的最后一个取出一个元素，作为当前要处理的状态,并获取对应的NFA子集
		int nowIndex = noOrder[noOrder.size() - 1];
		nowStatus = nodeList[nowIndex]->getNFASet();
		noOrder.pop_back();
		//从符号表中遍历每个符号，生成新的子集
		map<char, set<char>>::iterator iter = alphabet->begin();//
		while (iter != alphabet->end())
		{
			set<int> newSet = closure(move(nowStatus, iter->first, nfa), nfa);
			if (!newSet.empty()) {
				//检查该集合是否已存在在节点表中
				bool exist = false;
				//如果已存在，存储这个结点的序号
				int alreadyExistNum;
				map<int, DFANode*>::iterator checkIter = nodeList.begin();
				while (checkIter != nodeList.end()) {
					if (checkIter->second->getNFASet() == newSet) {
						alreadyExistNum = checkIter->first;
						exist = true;
						break;
					}
					checkIter++;
				}
				//如果集合不存在在节点表中且非空，就把它插入节点表和终态表
				if (!exist) {
					num++;
					nodeList.insert(pair<int, DFANode*>(num, new DFANode(num, newSet)));
					noOrder.push_back(num);

					//如果集合包含终态，就把它插入终态表
					if (newSet.count(nfa->getZIndex()) != 0)
						zIndex.push_back(num);

					alreadyExistNum = num;
				}
				nodeList[nowIndex]->addConditionNode(new ConditionNode(alreadyExistNum, iter->first));
			}
			iter++;
		}
	}
}

set<int> DFA::move(set<int> s, char condition, NFA* nfa) {
	//定义结果集合
	set<int> result;
	//获取NFA节点表
	map<int, Nodetransform*> nfaNodeMap = nfa->getNodeMap();
	//定义迭代器
	set<int>::iterator iter = s.begin();
	while (iter != s.end()) {
		//获取当前要匹配的NFA结点的Node对象
		Nodetransform* nowNode = nfaNodeMap[*iter];
		//遍历这个对象的邻接表，条件符合就加入结果集合中
		ConditionNode* nowAdjointNode = nowNode->getConditionNode();
		while (nowAdjointNode != NULL) {
			if (nowAdjointNode->condition == condition)
				result.insert(nowAdjointNode->index);
			nowAdjointNode = nowAdjointNode->getnext();
		}
		iter++;
	}
	return result;
}

set<int> DFA::closure(set<int> s, NFA* nfa) {
	//定义结果集合
	set<int> result;
	//获取NFA节点表
	map<int, Nodetransform*> nfaNodeMap = nfa->getNodeMap();
	//定义迭代器
	set<int>::iterator iter = s.begin();
	while (iter != s.end()) {
		//先插入自己
		result.insert(*iter);
		//获取当前要匹配的NFA结点的Node对象
		Nodetransform* nowNode = nfaNodeMap[*iter];
		//遍历这个对象的邻接表，条件符合就加入结果集合中
		ConditionNode* nowAdjointNode = nowNode->getConditionNode();
		while (nowAdjointNode != NULL) {
			if (nowAdjointNode->condition == 'c') {
				result.insert(nowAdjointNode->index);
				//使用新得到的结点序号继续递归
				set<int> newS;
				newS.insert(nowAdjointNode->index);
				set<int> sonRusult = closure(newS, nfa);
				result.insert(sonRusult.begin(), sonRusult.end());
			}
			nowAdjointNode = nowAdjointNode->getnext();
		}
		iter++;
	}
	return result;
}

int main()
{

}
