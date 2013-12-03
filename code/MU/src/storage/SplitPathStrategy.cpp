#include "SplitPathStrategy.h"

SplitPathStrategy::SplitPathStrategy()
{
}

SplitPathStrategy::~SplitPathStrategy()
{
}

int SplitPathStrategy::PutEntry(string pathname, const char* buf, int n)
{
	/* find the p_fid */
	string ParentDir = pathname;
	string filename = pathname;
	uint64_t p_fid = FindID(ParentDir);
	

	/* make the key */
	string bucket = pathname;
	string user = pathname;
	string userId = user;
	string path = pathname;

	string key = userId + p_fid + filename;

	/* make the value */
	//was ready

	/* insert to db */
}

int SplitPathStrategy::GetEntry(string pathname, char *buf, int *n)
{
	/* find the p_fid */

	/* make the key */

	/* get from db */
}

int SplitPathStrategy::DeleteEntry(string pathname)
{
	/* find the p_fid */

	/* make the key */

	/* delete from db */
}

int SplitPathStrategy::FindID(string pathname)
{
	/* init */

	/* traverse the dir from pathname */

	/* traverse end, then find the final dir */
	
}