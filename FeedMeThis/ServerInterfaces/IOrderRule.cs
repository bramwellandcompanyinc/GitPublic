using System;
using System.Collections.Generic;

using FeedMeThis.Interfaces.Client;

namespace FeedMeThis.Interfaces.Server
{
	public interface IOrderRule
	{
		bool Apply(IRepository<IDishType> repo, /*List<IDishRequest> drRequests,*/ ref List<IDishDisplay> ddOutput);
	}
}
