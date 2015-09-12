var _str = function(_elem, _value)
{
	if (_elem.tagName == 'A')
	{
	}
	else if (_elem.tagName == 'INPUT')
	{
	}
	else if (_elem.tagName == 'SELECT')
	{
	}
	else
	{
		while (_elem.hasChildNodes())
		{
			_elem.removeChild(_elem.childNodes[0]);
		}
		
		_elem.appendChild(document.createTextNode(decodeURIComponent(escape(_value))));
	}
};

var _arr = function(_elem, _item)
{
	if (_item.length == 0)
	{
		return;
	}

	for (var _key in _item[0])
	{
		var _elems = _elem.getElementsByClassName(_key);
		for (var i = 0; i < _elems.length; ++i)
		{
			_str(_elems[i], _item[i][_key]);
		}
	}
};

var _map = function(_elem, _item)
{
	for (var _key in _item)
	{
		var _elems = _elem.getElementsByClassName(_key);
		for (var i = 0; i < _elems.length; ++i)
		{
			_str(_elems[i], _item[_key]);
		}
	}
};

var _ = function(_json)
{
	for (var _key in _json)
	{
		if (!_json[_key])
		{
			continue;
		}
		else if (_key == '_error')
		{
			if (_json['_error']['_code'] != '0')
			{
				alert(_json['_error']['_message']);
				return;
			}
		}
		else if (_json[_key].constructor === 'str'.constructor)
		{
			var _elems = document.getElementsByClassName(_key);
			for (var i = 0; i < _elems.length; ++i)
			{
				_str(_elems[i], _json[_key]);
			}
		}
		else if (_json[_key].constructor === [].constructor)
		{
			var _elems = document.getElementsByClassName(_key);
			for (var i = 0; i < _elems.length; ++i)
			{
				_arr(_elems[i], _json[_key]);
			}
		}
		else if (_json[_key].constructor === {}.constructor)
		{
			var _elems = document.getElementsByClassName(_key);
			for (var i = 0; i < _elems.length; ++i)
			{
				_map(_elems[i], _json[_key]);
			}
		}
	}
};
