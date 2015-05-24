using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FeedMeThis.Interfaces.Client;

namespace FeedMeThis.RequestParsers
{
	public class XmlInputDishRequestParser : IDishTypeInputParser
	{
		private const string strNotImplementedMessage = "The XmlInputDishRequestParser is not implemented";

		public XmlInputDishRequestParser() { throw new NotImplementedException("strNotImplementedMessage"); }
		public bool Parse(object domXmlDocument, out List<IDishRequest> items, out string strValidationFaultMessage) { throw new NotImplementedException("strNotImplementedMessage"); }
	}
}
