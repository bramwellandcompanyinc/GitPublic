using System;
using System.Threading;
using Microsoft.Practices.Unity;

using FeedMeThis.Interfaces.Client;
using FeedMeThis.Interfaces.Server;


namespace FeedMeThis.Framework
{
	public static class Factory
	{
		private static UnityContainer _container = new UnityContainer();

		//Provides a place for client code to register all concrete implementations
		//that it will need.  This doesn't need to be thread safe for the sake of
		//this exercise as the project is single-threaded but I'm doing it anyway.

		public static IUnityContainer Container
		{
			get { return _container; }
		}

		public static void RegisterType<T, C>() where C : T
		{
			lock (_container)
			{
				InjectionMember[] im = null;
				_container.RegisterType<T, C>("", im);
			}
		}

		public static T Instantiate<T>()
		{
			T instance = default(T);
			instance = _container.Resolve<T>("", null);

			return instance;
		}

	}
}
