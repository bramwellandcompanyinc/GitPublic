using System.Collections.Generic;
using FeedMeThese.Interfaces;

namespace FeedMeThese.Entities
{
	public partial class DishDisplay : IDishDisplay
	{
		private uint _count = 0;
		private bool _error = false;
		private DishDefinition _dishDefinitionReference = null;

		public DishDisplay() { }

		public DishDisplay(uint count, bool error, DishDefinition dishDefinitionReference)
		{
			_count = count;
			_error = error;
			_dishDefinitionReference = dishDefinitionReference;
		}

		public DishDefinition DefinitionRef
		{
			get { return _dishDefinitionReference; }
			set { _dishDefinitionReference = value; }
		}

		public override string ToString()
		{
			if (_error)
				return "error";
			else
			{
				string strDisplay = _dishDefinitionReference.Name;
				strDisplay += AppendItemCount();
				return strDisplay;
			}
		}

		public uint Count
		{
			get { return _count;}
			set { _count = value; }
		}

		public IComparer<IDishDisplay> Comparer
		{
			get { return new DishDisplayComparer(); }
		}

		//If we have more than one item of a given type, display the item count
		//as follows: "item (x3)"
		private string AppendItemCount()
		{
			string result = "";

			if (_count > 1)
				result = "(x" + _count.ToString() + ")";

			return result;
		}
	}
}