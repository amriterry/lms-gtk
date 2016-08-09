#ifndef _ISSUE_H_
#define _ISSUE_H_

#include <system/database/model/Model.h>

using namespace std;
using namespace tuber;

class Issue: public Model{
public:
	Issue();

	static QueryRow getIssueInfoOfCopy(int copyId);
	static void getIssues(RowCallback callback,Bundle* data = nullptr);
	static void deleteForCopies(vector<int> copyIds);
	static void deleteForUser(int userId);
	static bool hasIssuesForUser(int userId);

	static double calculateFine(string deadlineDate);

	constexpr static int ISSUE_LIMIT = 5;
	constexpr static int STATUS_ISSUED = 1;
	constexpr static int STATUS_RETURNED = 2;
	constexpr static int STATUS_RENEWED = 3;
	constexpr static int DEADLINE_FIRST = 14;
	constexpr static int DEADLINE_SECOND = 21;
	constexpr static int DEADLINE_THIRD = 30;
	constexpr static double FINE_RATE_FIRST = 1.0;
	constexpr static double FINE_RATE_SECOND = 5.0;
	constexpr static double FINE_RATE_THIRD = 10.0;
};

#endif
