#pragma once
#include <iostream>
#include <array>
#include <stack>
#include <queue>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <set>
#include <list>

template <typename T>
class Tree
{
public:
	std::string Name;
	T Item;
	int Level;
	Tree* Parent;
	std::list<Tree*> Childs;

	const Tree* Root;

public:
	// 루트 트리거 설정
	// 루트는 공백의 Name과 -1의 Level로 식별
	Tree() {
		Name = "";
		Level = -1;
		Parent = nullptr;

		Root = this;
	}

	// 값 추가, 새로운 값에 대한 정의는 지원 X
	void Insert(Tree* newTree)
	{
		if (newTree)
		{
			Childs.push_back(newTree);
		}
	}

	// 파라미터 뼈대와 더불어 하위 뼈대까지 모두 제거
	void Remove(Tree* delTree)
	{
		std::vector<Tree*> RemoveStack;
		RemoveStack.push_back(delTree);

		// 재귀없이 스택으로 하위탐색
		Tree* pointTree = nullptr;
		int offset = 0;
		
		while (offset < RemoveStack.size())
		{
			pointTree = RemoveStack[offset++];

			for (Tree* child : pointTree->Childs)
			{
				RemoveStack.push_back(child);
			}
		}

		for (int idx = 0; idx < RemoveStack.size(); idx++)
		{
			delete RemoveStack[idx];
		}

		delTree = nullptr;
	}

	// 루트에서 하위 경로로 Name과 똑같은 오브젝트를 찾음.
	Tree* Find(std::string Name)
	{
		std::vector<Tree<T>*> TreeStack;
		TreeStack.push_back((Tree<T> *)Root);

		// 재귀없이 스택으로 하위탐색
		Tree* pointTree = nullptr;
		int offset = 0;

		while (offset < TreeStack.size())
		{
			pointTree = TreeStack[offset++];

			for (Tree* child : pointTree->Childs)
			{
				TreeStack.push_back(child);
				if (child->Name == Name)
					return child;
			}
		}

		return nullptr;
	}

	// Root의 하위 오브젝트 모두 검출
	void GetAllChilds(Tree* Root, std::vector<Tree*>& TreeStack)
	{
		TreeStack.push_back(Root);
		for (Tree* child : Root->Childs)
		{
			GetAllChilds(child, TreeStack);
		}
	}

	// 모든 하이어라키의 오브젝트 개수 검출
	int GetObjectCount()
	{
		std::vector<Tree<T>*> TreeStack;
		TreeStack.push_back((Tree<T> *)Root);

		// 재귀없이 스택으로 하위탐색
		Tree* pointTree = nullptr;
		int offset = 0;

		while (offset < TreeStack.size())
		{
			pointTree = TreeStack[offset++];

			for (Tree* child : pointTree->Childs)
			{
				TreeStack.push_back(child);
			}
		}

		return (int)TreeStack.size() - 1;
	}

	// ParentName과 동일한 부모가 있다면 True 아니면 False
	bool IsParent(std::string ParentName)
	{
		Tree<T>* t = Parent;

		while (t)
		{
			if (t->Name == ParentName)
			{
				return true;
			}

			t = t->Parent;
		}

		return false;
	}
};

