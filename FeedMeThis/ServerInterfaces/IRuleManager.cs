using System;
using System.Collections.Generic;

//using FeedMeThis.Interfaces.Server;

namespace FeedMeThis.Interfaces.Server
{
	public interface IRuleManager
	{
		IOrderRule[] GetRules();
	}
}
