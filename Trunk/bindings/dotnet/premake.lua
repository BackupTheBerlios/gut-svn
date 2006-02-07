package.name = "GameGuts_DotNet"
package.kind = "dll"
package.language = "c#"
package.target = "Sim8.GameGuts"

package.defines =
{
	"TRACE"
}

package.links =
{
	"System"
}

package.files =
{
	matchfiles("*.cs")
}
