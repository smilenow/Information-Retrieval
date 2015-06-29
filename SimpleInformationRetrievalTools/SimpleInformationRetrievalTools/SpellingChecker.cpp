#include "SpellingChecker.h"

int SpellingChecker::CalculateEditDist(const string & a, const string & b)
{
	int dist;
	int **mat;
	int a_length = a.length();
	int b_length = b.length();

	mat = new int *[a_length + 1];
	for (int i = 0; i <= a_length; ++i)
	{
		mat[i] = new int[b_length + 1];
	}

	for (int i = 0; i <= b_length; ++i)
	{
		mat[0][i] = i;
	}

	for (int i = 1; i <= a_length; ++i)
	{
		mat[i][0] = i;
	}

	for (int i = 1; i <= a_length; ++i)
	{
		for (int j = 1; j <= b_length; ++j)
		{
			if (a[i - 1] == b[j - 1])
			{
				mat[i][j] = min({mat[i - 1][j] + 1, mat[i][j - 1] + 1, mat[i - 1][j - 1]});
			}
			else
			{
				mat[i][j] = min({mat[i - 1][j] + 1, mat[i][j - 1] + 1, mat[i - 1][j - 1] + 1});
			}
		}
	}

	dist = mat[a_length][b_length];

	for (int i = 0; i <= a_length; ++i)
	{
		delete [] mat[i];
	}
	delete [] mat;

	return dist;
}

string SpellingChecker::CheckWord(const string & queryword)
{
	string result;
	int resultfreq = 0;
	int minDist = 100;
	int dist;

	for (auto &word : dict)
	{
		if (word.first == queryword)
		{
			return queryword;
		}
	}

	for (auto &word : dict)
	{
		dist = CalculateEditDist(word.first, queryword);
		if (dist < minDist)
		{
			result = word.first;
			resultfreq = word.second;
			minDist = dist;
		}
		else if (dist == minDist)
		{
			if (word.second > resultfreq)
			{
				result = word.first;
				resultfreq = word.second;
			}
		}
	}

	if (minDist <= 3)
	{
		return result;
	}
	else
	{
		return queryword;
	}
}

vector< pair<string, string> > SpellingChecker::CheckQuery(vector< pair<string, string> > &query)
{
	int isCorrected = false;
	string t;
	for (auto &item : query)
	{
		t = CheckWord(item.second);
		if (t != item.second)
		{
			isCorrected = true;
		}
		item.second = t;
	}

	if (isCorrected)
	{
		cout << "Do You Mean : ";
		for (auto &item : query)
		{
			cout << item.second << ' ';
		}
		cout << endl;
	}

	return query;
}

int SpellingChecker::LoadDict()
{
	ifstream in;
	string line;
	int pos;

	in.open("FrequencyDict.txt", ios::in);
	if (!in.is_open())
	{
		cerr << "Can\'t open dict!" << endl;
		in.close();
		return 1;
	}

	auto word = dict.begin();
	while (getline(in, line))
    {
        pos = line.find_last_of(':');
        dict.insert(word, pair<string, int>(line.substr(0, pos), stoi(line.substr(pos + 1))));
    }

    in.close();
	return 0;
}

SpellingChecker::SpellingChecker()
{
	LoadDict();
}