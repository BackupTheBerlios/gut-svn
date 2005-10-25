package.name = "Test_Input"
package.kind = "exe"
package.language = "c++"
package.target = "test_input"

package.buildflags =
{
	"no-rtti"
}

package.includepaths =
{
	"../../code/include"
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
