var _ = function(json)
{
	if (json.status.code != 2000)
	{
		alert(json.status.message);
		return;
	}
	
	for (var item in json.session)
	{
		var _sessions = document.getElementsByClassName('_session');
		for (var i = 0; i < _sessions.length; ++i)
		{
			var _items = _sessions[i].getElementsByClassName('_' + item);
			for (var j = 0; j < _items.length; ++j)
			{
				_items[j].appendChild(document.createTextNode(decodeURIComponent(escape(json.session[item]))));
			}
		}
	}
};
