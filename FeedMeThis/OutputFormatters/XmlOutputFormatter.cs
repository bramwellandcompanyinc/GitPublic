using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

using FeedMeThis.Interfaces.Client;

namespace FeedMeThis.OutputFormatters
{
	//Provided only to show the idea that output could well go to a machine interface, not just a human reader

	public class XmlOutputFormatter : IOutputFormatter
	{
		private const string strNotImplementedMessage = "The XmlOutputFormatter is not implemented";
		public XmlOutputFormatter() { throw new NotImplementedException(strNotImplementedMessage); }
		public TextWriter FormatOutput(List<IDishDisplay> items) { throw new NotImplementedException(strNotImplementedMessage); }
	}
}
