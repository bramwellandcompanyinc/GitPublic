using System;
using System.Collections.Generic;
using System.Text;

namespace FeedMeThis.Interfaces.Client
{

	public interface IDishTypeInputParser
	{
		bool Parse(object input, out List<IDishRequest> items, out string strValidationFaultMessage);
	}
}
