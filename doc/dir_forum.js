//_items[j].appendChild(document.createTextNode(decodeURIComponent(escape(json.session[item]))));

var _ = function(_elem, _json)
{
	for (var _key in _json)
	{
		if (typeof _json[_key] == 'string')
		{
			_elems = _elems.getElementsByClassName(_key);
			for (_item in _elems)
			{
				alert(typeof _item);
			}
		}
		/*
		else
		{
			_(_elem, _json[_key]);
		}
		*/
	}
};
