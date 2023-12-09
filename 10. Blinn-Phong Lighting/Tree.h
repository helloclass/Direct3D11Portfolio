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
	// ��Ʈ Ʈ���� ����
	// ��Ʈ�� ������ Name�� -1�� Level�� �ĺ�
	Tree() {
		Name = "";
		Level = -1;
		Parent = nullptr;

		Root = this;
	}

	// �� �߰�, ���ο� ���� ���� ���Ǵ� ���� X
	void Insert(Tree* newTree)
	{
		if (newTree)
		{
			Childs.push_back(newTree);
		}
	}

	// �Ķ���� ����� ���Ҿ� ���� ������� ��� ����
	void Remove(Tree* delTree)
	{
		std::vector<Tree*> RemoveStack;
		RemoveStack.push_back(delTree);

		// ��;��� �������� ����Ž��
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

	// ��Ʈ���� ���� ��η� Name�� �Ȱ��� ������Ʈ�� ã��.
	Tree* Find(std::string Name)
	{
		std::vector<Tree<T>*> TreeStack;
		TreeStack.push_back((Tree<T> *)Root);

		// ��;��� �������� ����Ž��
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

	// Root�� ���� ������Ʈ ��� ����
	void GetAllChilds(Tree* Root, std::vector<Tree*>& TreeStack)
	{
		TreeStack.push_back(Root);
		for (Tree* child : Root->Childs)
		{
			GetAllChilds(child, TreeStack);
		}
	}

	// ��� ���̾��Ű�� ������Ʈ ���� ����
	int GetObjectCount()
	{
		std::vector<Tree<T>*> TreeStack;
		TreeStack.push_back((Tree<T> *)Root);

		// ��;��� �������� ����Ž��
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

	// ParentName�� ������ �θ� �ִٸ� True �ƴϸ� False
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

