using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FeedMeThis.Interfaces.Client
{
	public interface IDishDisplay
	{
		IDishType TypeRef { get; set; }
		string DisplayAs { get; }
		uint Count { get; set; }
		bool IsLastItem { get; set; }
		bool IsInError { get; set; }
	}
}
