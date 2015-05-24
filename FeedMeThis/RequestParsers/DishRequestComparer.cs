using System;
using System.Collections.Generic;

using FeedMeThis.Interfaces.Client;

namespace FeedMeThis.RequestParsers
{
	class DishRequestComparer : IComparer<IDishRequest>
	{
		public int Compare(IDishRequest x, IDishRequest y)
		{
			int result = 0;

			if (x.Index > y.Index)
				result = 1;
			else if (x.Index < y.Index)
				result = -1;

			return result;
		}
	}
}
