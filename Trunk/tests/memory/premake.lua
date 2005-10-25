package.name = "Test_Memory"
package.kind = "exe"
package.language = "c++"
package.target = "test_memory"

package.buildflags =
{
	"no-rtti"
}

package.includepaths =
{
	"../../code/include",
	"../../code/lib"
}

package.config["Debug"].libpaths = 
{
	"../../bin/debug"
}

package.config["Release"].libpaths =
{
	"../../bin/release"
}

package.links =
{
	"GameGuts"
}

package.files =
{
	matchfiles("*.cpp");
}
