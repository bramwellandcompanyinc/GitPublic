using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FeedMeThese.Interfaces
{
	public interface IRepository<T>
	{
		IEnumerable<T> GetAll();
		T GetById(int index);
		T GetBy2DSearch<TA, TB>(TA param1, TB param2);
		T GetByName(string name);
	}
}
