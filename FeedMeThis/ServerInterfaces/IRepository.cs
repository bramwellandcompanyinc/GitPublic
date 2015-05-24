using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FeedMeThis.Interfaces.Server
{
	public interface IRepository<T>
	{
		IEnumerable<T> GetAll();
		T GetById(int index);
		T GetBy2DSearch(object param1, object param2);
		T GetByName(string name);
	}
}
