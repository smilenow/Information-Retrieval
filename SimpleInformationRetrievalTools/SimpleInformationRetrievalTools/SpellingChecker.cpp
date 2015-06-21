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

	return result;
}

vector<string> &SpellingChecker::SplitAndCheck(const string & query, char delimiter, vector<string> & words)
{
	stringstream querystream(query);
	string item;
	while (getline(querystream, item, delimiter)) {
		if (!item.empty())
		{
			words.push_back(CheckWord(item));
		}
	}
	return words;
}

vector<string> SpellingChecker::CheckQuery(const string & query)
{
	vector<string> words;
	SplitAndCheck(query, ' ', words);

	return words;
}

int SpellingChecker::LoadDict()
{
	// json version using rapidjson
	// FILE* fp = fopen("dict.json", "rb"); // 非Windows平台使用"r"
	// char readBuffer[65536];
	// rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	// rapidjson::Document d;
	// d.ParseStream(is);

	// auto word = dict.begin();
	// for (auto iter = d.MemberBegin(); iter != d.MemberEnd(); ++iter)
	// {
	// 	dict.insert(word, pair<string, int>(iter->name.GetString(), iter->value.GetInt()));
	// }

	// fclose(fp);

	// custom parsing

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
        dict.insert(word, pair<string, int>(line.substr(0, pos), atoi(line.substr(pos + 1).c_str())));
        // MingW does not have stoi
        // dict.insert(word, pair<string, int>(line.substr(0, pos), stoi(line.substr(pos + 1))));
    }

    in.close();
	return 0;
}

SpellingChecker::SpellingChecker()
{
	LoadDict();
}

/*int main(int argc, char const *argv[])
{
	SpellingChecker * SC = new SpellingChecker();
	while(true) {
		cin >> temp;
	    cout << SC->CheckWord(temp);
	}
	return 0;
}*/