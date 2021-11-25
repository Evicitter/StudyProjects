//Last update time : 18.07.2015 13:32:22
#pragma once

#include "ComChannel.h"
#include <string>
#include <vector>
#include <stack>
#include "Protocols.h"

using namespace std;

class GLOG
{
public:
	GLOG(wstring portname);
	virtual ~GLOG();

	ComChannel cc;
	Protocol* prot;

	virtual int Run();
};