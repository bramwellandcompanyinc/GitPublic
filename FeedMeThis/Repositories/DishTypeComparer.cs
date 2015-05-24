using System;
using System.Collections.Generic;

using FeedMeThis.Interfaces.Client;

namespace FeedMeThis.Repositories
{
	class DishTypeComparer : IComparer<IDishType>
	{
		public int Compare(IDishType x, IDishType y)
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
