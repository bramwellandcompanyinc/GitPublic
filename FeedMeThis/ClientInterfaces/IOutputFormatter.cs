using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace FeedMeThis.Interfaces.Client
{
	public interface IOutputFormatter
	{
		TextWriter FormatOutput(List<IDishDisplay> items);
	}
}
