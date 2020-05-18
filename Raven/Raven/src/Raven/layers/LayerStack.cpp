#include <pch.h>
#include "LayerStack.h"

namespace rvn {
	LayerStack::~LayerStack()
	{
	}
	void LayerStack::pushLayer(Layer* layer)
	{
		_layers.insert(_layers.begin() + _layerInsertIndex, layer);
		layer->onAttach();
		_layerInsertIndex++;
	}
	void LayerStack::pushOverlay(Layer* overlay)
	{
		_layers.push_back(overlay);
		overlay->onAttach();
	}
	void LayerStack::popLayer(Layer* layer)
	{
		auto it = std::find(_layers.begin(), _layers.begin() + _layerInsertIndex, layer);
		if (it != _layers.end()) {
			layer->onDetach();
			_layers.erase(it);
			_layerInsertIndex--;
		}
	}
	void LayerStack::popOverlay(Layer* overlay)
	{
		auto it = std::find(_layers.begin() + _layerInsertIndex, _layers.end(), overlay);
		if (it != _layers.end()) {
			overlay->onDetach();
			_layers.erase(it);
		}
	}
}