using System;
using System.Collections.Generic;

using FeedMeThis.Interfaces.Client;

namespace FeedMeThis.Repositories
{
	public class DishDisplay : IDishDisplay
	{
		private uint _count = 0;
		private bool _error= false;
		private bool _islastitem = false;
		private IDishType _typeref = null;

		public DishDisplay() { }

		public DishDisplay(uint count, bool error, IDishType typeref)
		{
			_count = count;
			_error = error;
			_typeref = typeref;
		}

		public IDishType TypeRef
		{
			get { return _typeref; }
			set { _typeref = value; }
		}

		public string DisplayAs
		{
			get
			{
				if (_error)
					return "error";
				else
				{
					string strDisplay = _typeref.Name;
					strDisplay += AppendItemCount();
					return strDisplay;
				}
			}
		}

		public uint Count
		{
			get { return _count;}
			set { _count = value; }
		}

		public bool IsLastItem
		{
			get { return _islastitem; }
			set { _islastitem = value; }
		}

		public bool IsInError
		{
			get { return _error; }
			set { _error = value; }
		}

		//If we have more than one item of a given type, display the item count
		//as follows: "item (x3)"
		private string AppendItemCount()
		{
			string result = "";

			if (_count > 1)
				result = " (x" + _count.ToString() + ")";

			return result;
		}
	}
}
