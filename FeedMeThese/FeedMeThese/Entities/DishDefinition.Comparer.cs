using System.Collections.Generic;

namespace FeedMeThese.Entities
{
	class DishDefinitionComparer : IComparer<DishDefinition>
	{
		public int Compare(DishDefinition x, DishDefinition y)
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
