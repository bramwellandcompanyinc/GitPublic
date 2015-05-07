using System.Collections.Generic;
using FeedMeThese.Interfaces;

namespace FeedMeThese.Entities
{
	public partial class DishDisplay : IDishDisplay
	{
		private class DishDisplayComparer : IComparer<IDishDisplay>
		{
			public int Compare(IDishDisplay x, IDishDisplay y)
			{
				int result = 0;

				//These first two tests make sure that error items always come last in the sort order
				//because they'll never have a definition reference set

				if (x.DefinitionRef == null && y.DefinitionRef != null)
					result = 1;
				else if (y.DefinitionRef == null && x.DefinitionRef != null)
					result = -1;
				else if (x.DefinitionRef.Index > y.DefinitionRef.Index)
					result = 1;
				else if (x.DefinitionRef.Index < y.DefinitionRef.Index)
					result = -1;

				return result;
			}
		}
	}
}