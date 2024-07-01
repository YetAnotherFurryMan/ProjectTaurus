#include <stdio.h>
#include <string.h>

#include <trsap/trsap.h>

typedef struct Case{
	const int argc;
	const char** argv;
	const size_t count;
	const char** lines;
	const trsap_ArgStatus endStatus;
} Case;

const Case cases[211] = {
	{
		1, (const char*[]){"-"},
		1, (const char*[]){
			"Arg[-1]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_SHORT
	},
	{
		1, (const char*[]){"-f"},
		1, (const char*[]){
			"Arg[-1]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_SHORT
	},
	{
		1, (const char*[]){"--ff"},
		1, (const char*[]){
			"Arg[-1]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_LONG
	},

	{
		1, (const char*[]){"a"},
		2, (const char*[]){
			"Arg[-1]: \"a\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-a"},
		2, (const char*[]){
			"Arg[a]: \"(null)\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-b"},
		1, (const char*[]){
			"Arg[ab]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_VALUE
	},
	{
		1, (const char*[]){"-b:v1"},
		2, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-b=v1"},
		2, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-bv1"},
		2, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-c"},
		2, (const char*[]){
			"Arg[ac]: \"(null)\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-c:v1"},
		2, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-c=v1"},
		2, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-cv1"},
		2, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-d"},
		1, (const char*[]){
			"Arg[ad]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_VALUE
	},
	{
		1, (const char*[]){"-d:v1"},
		1, (const char*[]){
			"Arg[ad]: \"v1\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_VALUE2
	},
	{
		1, (const char*[]){"-d=v1"},
		1, (const char*[]){
			"Arg[ad]: \"v1\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_VALUE2
	},
	{
		1, (const char*[]){"-d:v1:v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-d:v1=v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-d=v1:v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-d=v1=v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-dv1"},
		1, (const char*[]){
			"Arg[ad]: \"v1\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_VALUE2
	},
	{
		1, (const char*[]){"-dv1"},
		1, (const char*[]){
			"Arg[ad]: \"v1\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_VALUE2
	},
	{
		1, (const char*[]){"-dv1:v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-dv1=v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-e"},
		1, (const char*[]){
			"Arg[ee]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_VALUE
	},
	{
		1, (const char*[]){"-e:v1"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-e=v1"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-ev1"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-e:v1:v2"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-e:v1=v2"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-e=v1:v2"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-e=v1=v2"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-ev1:v2"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"-ev1=v2"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"--a"},
		2, (const char*[]){
			"Arg[a]: \"(null)\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"--ab"},
		1, (const char*[]){
			"Arg[ab]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_VALUE
	},
	{
		1, (const char*[]){"--ab:v1"},
		2, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"--ab=v1"},
		2, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"--ac"},
		2, (const char*[]){
			"Arg[ac]: \"(null)\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"--ac:v1"},
		2, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"--ac=v1"},
		2, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"--ad"},
		1, (const char*[]){
			"Arg[ad]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_VALUE
	},
	{
		1, (const char*[]){"--ad:v1"},
		1, (const char*[]){
			"Arg[ad]: \"v1\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_VALUE2
	},
	{
		1, (const char*[]){"--ad=v1"},
		1, (const char*[]){
			"Arg[ad]: \"v1\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_VALUE2
	},
	{
		1, (const char*[]){"--ad:v1:v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"--ad:v1=v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"--ad=v1:v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"--ad=v1=v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"--ee"},
		1, (const char*[]){
			"Arg[ee]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_VALUE
	},
	{
		1, (const char*[]){"--ee:v1"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"--ee=v1"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"--ee:v1:v2"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"--ee:v1=v2"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"--ee=v1:v2"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		1, (const char*[]){"--ee=v1=v2"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"a", "b"},
		3, (const char*[]){
			"Arg[-1]: \"a\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-a", "b"},
		3, (const char*[]){
			"Arg[a]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-b", "b"},
		2, (const char*[]){
			"Arg[ab]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-b:v1", "b"},
		3, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-b=v1", "b"},
		3, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-bv1", "b"},
		3, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-c", "b"},
		3, (const char*[]){
			"Arg[ac]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-c:v1", "b"},
		3, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-c=v1", "b"},
		3, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-cv1", "b"},
		3, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-d", "b"},
		1, (const char*[]){
			"Arg[ad]: \"b\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_VALUE2
	},
	{
		2, (const char*[]){"-d:v1", "b"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-d=v1", "b"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-d:v1:v2", "b"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-d:v1=v2", "b"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-d=v1:v2", "b"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-d=v1=v2", "b"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-dv1", "b"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-dv1", "b"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-dv1:v2", "b"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-dv1=v2", "b"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-e", "b"},
		2, (const char*[]){
			"Arg[ee]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-e:v1", "b"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-e=v1", "b"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-ev1", "b"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-e:v1:v2", "b"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-e:v1=v2", "b"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-e=v1:v2", "b"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-e=v1=v2", "b"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-ev1:v2", "b"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-ev1=v2", "b"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--a", "b"},
		3, (const char*[]){
			"Arg[a]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ab", "b"},
		2, (const char*[]){
			"Arg[ab]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ab:v1", "b"},
		3, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ab=v1", "b"},
		3, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ac", "b"},
		3, (const char*[]){
			"Arg[ac]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ac:v1", "b"},
		3, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ac=v1", "b"},
		3, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ad", "b"},
		1, (const char*[]){
			"Arg[ad]: \"b\" \"(null)\""
		},
		TRSAP_ARG_STATUS_ERR_VALUE2
	},
	{
		2, (const char*[]){"--ad:v1", "b"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ad=v1", "b"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ad:v1:v2", "b"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ad:v1=v2", "b"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ad=v1:v2", "b"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ad=v1=v2", "b"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ee", "b"},
		2, (const char*[]){
			"Arg[ee]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ee:v1", "b"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ee=v1", "b"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ee:v1:v2", "b"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ee:v1=v2", "b"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ee=v1:v2", "b"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ee=v1=v2", "b"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},

	{
		2, (const char*[]){"a", "v1:v2"},
		3, (const char*[]){
			"Arg[-1]: \"a\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-a", "v1:v2"},
		3, (const char*[]){
			"Arg[a]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-b", "v1:v2"},
		2, (const char*[]){
			"Arg[ab]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-b:v1", "v1:v2"},
		3, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-b=v1", "v1:v2"},
		3, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-bv1", "v1:v2"},
		3, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-c", "v1:v2"},
		3, (const char*[]){
			"Arg[ac]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-c:v1", "v1:v2"},
		3, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-c=v1", "v1:v2"},
		3, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-cv1", "v1:v2"},
		3, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-d", "v1:v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-d:v1", "v1:v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v1:v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-d=v1", "v1:v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v1:v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-d:v1:v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-d:v1=v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-d=v1:v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-d=v1=v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-dv1", "v1:v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v1:v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-dv1", "v1:v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v1:v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-dv1:v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-dv1=v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-e", "v1:v2"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-e:v1", "v1:v2"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-e=v1", "v1:v2"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-ev1", "v1:v2"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-e:v1:v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-e:v1=v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-e=v1:v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-e=v1=v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-ev1:v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"-ev1=v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--a", "v1:v2"},
		3, (const char*[]){
			"Arg[a]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ab", "v1:v2"},
		2, (const char*[]){
			"Arg[ab]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ab:v1", "v1:v2"},
		3, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ab=v1", "v1:v2"},
		3, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ac", "v1:v2"},
		3, (const char*[]){
			"Arg[ac]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ac:v1", "v1:v2"},
		3, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ac=v1", "v1:v2"},
		3, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ad", "v1:v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ad:v1", "v1:v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v1:v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ad=v1", "v1:v2"},
		2, (const char*[]){
			"Arg[ad]: \"v1\" \"v1:v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ad:v1:v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ad:v1=v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ad=v1:v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ad=v1=v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ee", "v1:v2"},
		2, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ee:v1", "v1:v2"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ee=v1", "v1:v2"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ee:v1:v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ee:v1=v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ee=v1:v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		2, (const char*[]){"--ee=v1=v2", "v1:v2"},
		3, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"a", "b", "c"},
		4, (const char*[]){
			"Arg[-1]: \"a\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-a", "b", "c"},
		4, (const char*[]){
			"Arg[a]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-b", "b", "c"},
		3, (const char*[]){
			"Arg[ab]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-b:v1", "b", "c"},
		4, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-b=v1", "b", "c"},
		4, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-bv1", "b", "c"},
		4, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-c", "b", "c"},
		4, (const char*[]){
			"Arg[ac]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-c:v1", "b", "c"},
		4, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-c=v1", "b", "c"},
		4, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-cv1", "b", "c"},
		4, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-d", "b", "c"},
		2, (const char*[]){
			"Arg[ad]: \"b\" \"c\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-d:v1", "b", "c"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-d=v1", "b", "c"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-d:v1:v2", "b", "c"},
		4, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-d:v1=v2", "b", "c"},
		4, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-d=v1:v2", "b", "c"},
		4, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-d=v1=v2", "b", "c"},
		4, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-dv1", "b", "c"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-dv1", "b", "c"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-dv1:v2", "b", "c"},
		4, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-dv1=v2", "b", "c"},
		4, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-e", "b", "c"},
		3, (const char*[]){
			"Arg[ee]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-e:v1", "b", "c"},
		4, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-e=v1", "b", "c"},
		4, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-ev1", "b", "c"},
		4, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-e:v1:v2", "b", "c"},
		4, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-e:v1=v2", "b", "c"},
		4, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-e=v1:v2", "b", "c"},
		4, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-e=v1=v2", "b", "c"},
		4, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-ev1:v2", "b", "c"},
		4, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"-ev1=v2", "b", "c"},
		4, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--a", "b", "c"},
		4, (const char*[]){
			"Arg[a]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ab", "b", "c"},
		3, (const char*[]){
			"Arg[ab]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ab:v1", "b", "c"},
		4, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ab=v1", "b", "c"},
		4, (const char*[]){
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ac", "b", "c"},
		4, (const char*[]){
			"Arg[ac]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ac:v1", "b", "c"},
		4, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ac=v1", "b", "c"},
		4, (const char*[]){
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ad", "b", "c"},
		2, (const char*[]){
			"Arg[ad]: \"b\" \"c\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ad:v1", "b", "c"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ad=v1", "b", "c"},
		3, (const char*[]){
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ad:v1:v2", "b", "c"},
		4, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ad:v1=v2", "b", "c"},
		4, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ad=v1:v2", "b", "c"},
		4, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ad=v1=v2", "b", "c"},
		4, (const char*[]){
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ee", "b", "c"},
		3, (const char*[]){
			"Arg[ee]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ee:v1", "b", "c"},
		4, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ee=v1", "b", "c"},
		4, (const char*[]){
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ee:v1:v2", "b", "c"},
		4, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ee:v1=v2", "b", "c"},
		4, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ee=v1:v2", "b", "c"},
		4, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
	{
		3, (const char*[]){"--ee=v1=v2", "b", "c"},
		4, (const char*[]){
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\"",
			"Arg[a]: \"(null)\" \"(null)\""
		},
		TRSAP_ARG_STATUS_EOI
	},
};

#define CASE_COUNT (sizeof(cases) / sizeof(Case))

int main(void){
	trsap_Desc descs[] = {
		{ 0, "-1", 0 },
		{ 'a', "a", TRSAP_ARG_TYPE_FLAG },
		{ 'b', "ab", TRSAP_ARG_TYPE_VALUE },
		{ 'c', "ac", TRSAP_ARG_TYPE_VALUE_OPTIONAL },
		{ 'd', "ad", TRSAP_ARG_TYPE_VALUE2 },
		{ 'e', "ee", TRSAP_ARG_TYPE_VALUE2_OPTIONAL }
	};

	trsap_Arg arg;
	size_t passed = 0;
	char buff[255] = {0};
	for(size_t casei = 0; casei < CASE_COUNT; casei++){
		int argc = cases[casei].argc;
		const char** argv = cases[casei].argv;
		size_t count = cases[casei].count;
		const char** lines = cases[casei].lines;

		do{
			arg = trsap_next(5, descs + 1, &argc, &argv);

			const char* val = (arg.m_ValueLen == 0)?"(null)":arg.m_Value;
			size_t vallen = (arg.m_ValueLen == 0)?6:arg.m_ValueLen;
			sprintf(buff, "Arg[%s]: \"%.*s\" \"%s\"", descs[arg.m_Id + 1].m_Long, (int)vallen, val, arg.m_Value2);
			
			if(strcmp(buff, *lines) == 0){
				count--;
			} else{
				fprintf(stderr, "FAILED(case %ld): Lines mismatch\n  G: %s\n  E: %s\n", casei, buff, *lines);
				break;
			}

			lines++;
		} while(arg.m_Status == TRSAP_ARG_STATUS_OK);

		if(count == 0 && arg.m_Status == cases[casei].endStatus)
			passed++;
		else
			fprintf(stderr, "FAILED(case %ld): End status mismatch\n  G: %ld:%d\n  E: 0:%d\n", casei, count, arg.m_Status, cases[casei].endStatus);
	}

	if(passed == CASE_COUNT)
		printf("OK\n");
	else
		printf("FAILED\n");
}

