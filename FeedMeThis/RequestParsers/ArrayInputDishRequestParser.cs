using System;
using System.Collections.Generic;
using System.Text;

using FeedMeThis.Interfaces.Client;

namespace FeedMeThis.RequestParsers
{

	public class ArrayInputDishRequestParser : IDishTypeInputParser
	{
		public ArrayInputDishRequestParser() { }

		public bool Parse(object strTextInput, out List<IDishRequest> items, out string strValidationFaultMessage)
		{
			throw new NotImplementedException("The ArrayInputDishRequestParser is provided as sample only and is not intended to be implemented.");
		}
	}
}
