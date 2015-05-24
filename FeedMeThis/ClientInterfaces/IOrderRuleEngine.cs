using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FeedMeThis.Interfaces.Client
{
	public interface IOrderRuleEngine
	{
		List<IDishDisplay> Process(List<IDishRequest> dtRequests);
	}
}
