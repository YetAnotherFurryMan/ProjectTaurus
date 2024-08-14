#include <iostream>
#include <sstream>
#include <vector>

#include <toollib/ap/ap.hpp>

namespace tl = toollib;

struct Case{
	std::vector<const char*> args;
	std::vector<std::string> lines;
	tl::ap::ArgStatus endStatus = tl::ap::ArgStatus::OK;
};

std::vector<Case> cases = {
	{
		{"-"},
		{
			"Arg[-1]: \"(null)\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_SHORT
	},
	{
		{"-f"},
		{
			"Arg[-1]: \"(null)\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_SHORT
	},
	{
		{"--ff"},
		{
			"Arg[-1]: \"(null)\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_LONG
	},

	{
		{"a"},
		{
			"Arg[-1]: \"a\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-a"},
		{
			"Arg[a]: \"(null)\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-b"},
		{
			"Arg[ab]: \"(null)\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_VALUE
	},
	{
		{"-b:v1"},
		{
			"Arg[ab]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-b=v1"},
		{
			"Arg[ab]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-bv1"},
		{
			"Arg[ab]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-c"},
		{
			"Arg[ac]: \"(null)\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-c:v1"},
		{
			"Arg[ac]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-c=v1"},
		{
			"Arg[ac]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-cv1"},
		{
			"Arg[ac]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d"},
		{
			"Arg[ad]: \"(null)\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_VALUE
	},
	{
		{"-d:v1"},
		{
			"Arg[ad]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_VALUE2
	},
	{
		{"-d=v1"},
		{
			"Arg[ad]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_VALUE2
	},
	{
		{"-d:v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d:v1=v2"},
		{
			"Arg[ad]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d=v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d=v1=v2"},
		{
			"Arg[ad]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-dv1"},
		{
			"Arg[ad]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_VALUE2
	},
	{
		{"-dv1"},
		{
			"Arg[ad]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_VALUE2
	},
	{
		{"-dv1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-dv1=v2"},
		{
			"Arg[ad]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e"},
		{
			"Arg[ee]: \"(null)\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_VALUE
	},
	{
		{"-e:v1"},
		{
			"Arg[ee]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e=v1"},
		{
			"Arg[ee]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-ev1"},
		{
			"Arg[ee]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e:v1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e:v1=v2"},
		{
			"Arg[ee]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e=v1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e=v1=v2"},
		{
			"Arg[ee]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-ev1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-ev1=v2"},
		{
			"Arg[ee]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--a"},
		{
			"Arg[a]: \"(null)\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ab"},
		{
			"Arg[ab]: \"(null)\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_VALUE
	},
	{
		{"--ab:v1"},
		{
			"Arg[ab]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ab=v1"},
		{
			"Arg[ab]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ac"},
		{
			"Arg[ac]: \"(null)\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ac:v1"},
		{
			"Arg[ac]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ac=v1"},
		{
			"Arg[ac]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad"},
		{
			"Arg[ad]: \"(null)\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_VALUE
	},
	{
		{"--ad:v1"},
		{
			"Arg[ad]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_VALUE2
	},
	{
		{"--ad=v1"},
		{
			"Arg[ad]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_VALUE2
	},
	{
		{"--ad:v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad:v1=v2"},
		{
			"Arg[ad]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad=v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad=v1=v2"},
		{
			"Arg[ad]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee"},
		{
			"Arg[ee]: \"(null)\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_VALUE
	},
	{
		{"--ee:v1"},
		{
			"Arg[ee]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee=v1"},
		{
			"Arg[ee]: \"v1\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee:v1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee:v1=v2"},
		{
			"Arg[ee]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee=v1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee=v1=v2"},
		{
			"Arg[ee]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"a", "b"},
		{
			"Arg[-1]: \"a\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-a", "b"},
		{
			"Arg[a]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-b", "b"},
		{
			"Arg[ab]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-b:v1", "b"},
		{
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-b=v1", "b"},
		{
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-bv1", "b"},
		{
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-c", "b"},
		{
			"Arg[ac]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-c:v1", "b"},
		{
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-c=v1", "b"},
		{
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-cv1", "b"},
		{
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d", "b"},
		{
			"Arg[ad]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_VALUE2
	},
	{
		{"-d:v1", "b"},
		{
			"Arg[ad]: \"v1\" \"b\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d=v1", "b"},
		{
			"Arg[ad]: \"v1\" \"b\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d:v1:v2", "b"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d:v1=v2", "b"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d=v1:v2", "b"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d=v1=v2", "b"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-dv1", "b"},
		{
			"Arg[ad]: \"v1\" \"b\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-dv1", "b"},
		{
			"Arg[ad]: \"v1\" \"b\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-dv1:v2", "b"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-dv1=v2", "b"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e", "b"},
		{
			"Arg[ee]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e:v1", "b"},
		{
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e=v1", "b"},
		{
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-ev1", "b"},
		{
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e:v1:v2", "b"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e:v1=v2", "b"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e=v1:v2", "b"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e=v1=v2", "b"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-ev1:v2", "b"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-ev1=v2", "b"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--a", "b"},
		{
			"Arg[a]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ab", "b"},
		{
			"Arg[ab]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ab:v1", "b"},
		{
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ab=v1", "b"},
		{
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ac", "b"},
		{
			"Arg[ac]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ac:v1", "b"},
		{
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ac=v1", "b"},
		{
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad", "b"},
		{
			"Arg[ad]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::ERR_VALUE2
	},
	{
		{"--ad:v1", "b"},
		{
			"Arg[ad]: \"v1\" \"b\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad=v1", "b"},
		{
			"Arg[ad]: \"v1\" \"b\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad:v1:v2", "b"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad:v1=v2", "b"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad=v1:v2", "b"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad=v1=v2", "b"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee", "b"},
		{
			"Arg[ee]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee:v1", "b"},
		{
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee=v1", "b"},
		{
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee:v1:v2", "b"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee:v1=v2", "b"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee=v1:v2", "b"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee=v1=v2", "b"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},

	{
		{"a", "v1:v2"},
		{
			"Arg[-1]: \"a\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-a", "v1:v2"},
		{
			"Arg[a]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-b", "v1:v2"},
		{
			"Arg[ab]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-b:v1", "v1:v2"},
		{
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-b=v1", "v1:v2"},
		{
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-bv1", "v1:v2"},
		{
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-c", "v1:v2"},
		{
			"Arg[ac]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-c:v1", "v1:v2"},
		{
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-c=v1", "v1:v2"},
		{
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-cv1", "v1:v2"},
		{
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d:v1", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v1:v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d=v1", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v1:v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d:v1:v2", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d:v1=v2", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d=v1:v2", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d=v1=v2", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-dv1", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v1:v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-dv1", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v1:v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-dv1:v2", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-dv1=v2", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e:v1", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e=v1", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-ev1", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e:v1:v2", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e:v1=v2", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e=v1:v2", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e=v1=v2", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-ev1:v2", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-ev1=v2", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--a", "v1:v2"},
		{
			"Arg[a]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ab", "v1:v2"},
		{
			"Arg[ab]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ab:v1", "v1:v2"},
		{
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ab=v1", "v1:v2"},
		{
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ac", "v1:v2"},
		{
			"Arg[ac]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ac:v1", "v1:v2"},
		{
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ac=v1", "v1:v2"},
		{
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad:v1", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v1:v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad=v1", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v1:v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad:v1:v2", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad:v1=v2", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad=v1:v2", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad=v1=v2", "v1:v2"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee:v1", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee=v1", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee:v1:v2", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee:v1=v2", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee=v1:v2", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee=v1=v2", "v1:v2"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"v1:v2\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"a", "b", "c"},
		{
			"Arg[-1]: \"a\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-a", "b", "c"},
		{
			"Arg[a]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-b", "b", "c"},
		{
			"Arg[ab]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-b:v1", "b", "c"},
		{
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-b=v1", "b", "c"},
		{
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-bv1", "b", "c"},
		{
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-c", "b", "c"},
		{
			"Arg[ac]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-c:v1", "b", "c"},
		{
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-c=v1", "b", "c"},
		{
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-cv1", "b", "c"},
		{
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d", "b", "c"},
		{
			"Arg[ad]: \"b\" \"c\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d:v1", "b", "c"},
		{
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d=v1", "b", "c"},
		{
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d:v1:v2", "b", "c"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d:v1=v2", "b", "c"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d=v1:v2", "b", "c"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-d=v1=v2", "b", "c"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-dv1", "b", "c"},
		{
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-dv1", "b", "c"},
		{
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-dv1:v2", "b", "c"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-dv1=v2", "b", "c"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e", "b", "c"},
		{
			"Arg[ee]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e:v1", "b", "c"},
		{
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e=v1", "b", "c"},
		{
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-ev1", "b", "c"},
		{
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e:v1:v2", "b", "c"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e:v1=v2", "b", "c"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e=v1:v2", "b", "c"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-e=v1=v2", "b", "c"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-ev1:v2", "b", "c"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"-ev1=v2", "b", "c"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--a", "b", "c"},
		{
			"Arg[a]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ab", "b", "c"},
		{
			"Arg[ab]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ab:v1", "b", "c"},
		{
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ab=v1", "b", "c"},
		{
			"Arg[ab]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ac", "b", "c"},
		{
			"Arg[ac]: \"(null)\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ac:v1", "b", "c"},
		{
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ac=v1", "b", "c"},
		{
			"Arg[ac]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad", "b", "c"},
		{
			"Arg[ad]: \"b\" \"c\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad:v1", "b", "c"},
		{
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad=v1", "b", "c"},
		{
			"Arg[ad]: \"v1\" \"b\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad:v1:v2", "b", "c"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad:v1=v2", "b", "c"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad=v1:v2", "b", "c"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ad=v1=v2", "b", "c"},
		{
			"Arg[ad]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee", "b", "c"},
		{
			"Arg[ee]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee:v1", "b", "c"},
		{
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee=v1", "b", "c"},
		{
			"Arg[ee]: \"v1\" \"(null)\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee:v1:v2", "b", "c"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee:v1=v2", "b", "c"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee=v1:v2", "b", "c"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
	{
		{"--ee=v1=v2", "b", "c"},
		{
			"Arg[ee]: \"v1\" \"v2\"",
			"Arg[-1]: \"b\" \"(null)\"",
			"Arg[-1]: \"c\" \"(null)\""
		},
		tl::ap::ArgStatus::EOI
	},
};

int main(){
	tl::ap::Desc descs[] = {
		{ 0, "-1", tl::ap::ArgType::FLAG },
		{ 'a', "a", tl::ap::ArgType::FLAG },
		{ 'b', "ab", tl::ap::ArgType::VALUE },
		{ 'c', "ac", tl::ap::ArgType::VALUE_OPTIONAL },
		{ 'd', "ad", tl::ap::ArgType::VALUE2 },
		{ 'e', "ee", tl::ap::ArgType::VALUE2_OPTIONAL }
	};

	size_t passed = 0;
	for(size_t casei = 0; casei < cases.size(); casei++){
		size_t lpassed = 0;
		
		int argc = cases[casei].args.size();
		const char* const* argv = cases[casei].args.data();
		auto args = tl::ap::getAll(5, descs + 1, &argc, &argv);

		tl::ap::ArgStatus es = tl::ap::ArgStatus::EOI;
		for(size_t i = 0; i < args.size(); i++){
			auto& arg = args[i];
			auto& line = cases[casei].lines[i];

			std::stringstream ss;

			ss << "Arg[" << descs[arg.m_Id + 1].m_Long << "]: \"";
			if(arg.m_Value && arg.m_ValueLen) 
				ss << std::string(arg.m_Value).substr(0, arg.m_ValueLen); 
			else 
				ss << "(null)";
			ss << "\" \"";
			if(arg.m_Value2 && arg.m_Value2Len) 
				ss << std::string(arg.m_Value2).substr(0, arg.m_Value2Len); 
			else 
				ss << "(null)";
			ss << "\"";
		
			if(ss.str() == line)
				lpassed++;
			else
				std::cerr << "FAILED(case " << casei << "): Lines dismatch" << std::endl << "  G: " << ss.str() << std::endl << "  E: " << line << std::endl;
		
			if(arg.m_Status != tl::ap::ArgStatus::OK){
				es = arg.m_Status;
				break;
			}
		}

		if(lpassed == cases[casei].lines.size() && es == cases[casei].endStatus)
			passed++;
		else
			std::cerr << "FAILED(case " << casei << "): End status dismatch" << std::endl << "  G: " << lpassed << ":" << (int)es << std::endl << "  E: " << cases[casei].lines.size() << ":" << (int)cases[casei].endStatus << std::endl;
	}
	
	if(passed == cases.size())
		std::cout << "OK" << std::endl; 
	else
		std::cout << "FAILED" << std::endl;

	return 0;
}

