#pragma once

#include<vector>
#include<memory>

#include<wrl.h>
#include<wrl/implements.h>
#include<wrl\client.h>
#include<windows.foundation.collections.h>

namespace Supplimental
{
	namespace WRL
	{
		namespace Collections {
			template <class T>
			class Iterator : public Microsoft::WRL::RuntimeClass<ABI::Windows::Foundation::Collections::IIterator<T>>
			{
				InspectableClass(L"Supplimental.WRL.Collections.Iterator", BaseTrust)

			private:
				typedef typename std::shared_ptr<std::vector<T>> V;
				typedef typename std::vector<T>::iterator IT;

			public:
				Iterator() {}

				HRESULT Init(const V & item)
				{
					_v = item;
					_it = _v->begin();
					if (_it != _v->end())
					{
						_element = *_it;
						_bElement = TRUE;
					}
					else
						_bElement = FALSE;
					return S_OK;
				}

			public:
				virtual HRESULT STDMETHODCALLTYPE get_Current(T *current)
				{
					if (_it != _v->end())
					{
						_bElement = TRUE;
						_element = *_it;
						*current = _element;
					}
					else
					{
						_bElement = FALSE;
					}
					return S_OK;
				}

				virtual HRESULT STDMETHODCALLTYPE get_HasCurrent(boolean *hasCurrent)
				{
					if (_bElement)
						*hasCurrent = TRUE;
					else
						*hasCurrent = FALSE;
					return S_OK;
				}

				virtual HRESULT STDMETHODCALLTYPE MoveNext(boolean *hasCurrent)
				{
					if (_it != _v->end())
					{
						++_it;
						if (_it == _v->end())
							*hasCurrent = FALSE;
						else
							*hasCurrent = TRUE;
					}
					else
					{
						*hasCurrent = FALSE;
					}
					return S_OK;
				}

			private:
				V _v;
				IT _it;
				T _element;
				boolean _bElement = FALSE;
			};

			template <typename T>
			class Vector : public Microsoft::WRL::RuntimeClass<ABI::Windows::Foundation::Collections::IVector<T>,
				ABI::Windows::Foundation::Collections::IIterable<T>>
			{
				InspectableClass(L"Supplimenta.WRL.Collections.Vector", BaseTrust)

			public:
				Vector()
				{
					std::shared_ptr<std::vector<T>> ptr(new std::vector<T>());
					_v = ptr;
				}

			public:

				// read methods
				virtual HRESULT STDMETHODCALLTYPE GetAt(unsigned index, T *item)
				{
					*item = (*_v)[index];
					return S_OK;
				}

				virtual HRESULT STDMETHODCALLTYPE get_Size(unsigned *size)
				{
					*size = _v->size();
					return S_OK;
				}

				virtual HRESULT STDMETHODCALLTYPE GetView(ABI::Windows::Foundation::Collections::IVectorView<T> **view)
				{
					return E_NOTIMPL;
				}

				virtual HRESULT STDMETHODCALLTYPE IndexOf(T value, unsigned *index, boolean *found)
				{
					unsigned foundedIndex = 0;
					bool founded = false;
					for (std::vector<T>::const_iterator it = _v->cbegin(); it != _v->end(); ++it)
					{
						const T & v = *it;
						if (v == value)
						{
							founded = true;
							break;
						}
						foundedIndex++;
					}
					if (founded == true)
					{
						*found = TRUE;
						*index = foundedIndex;
					}
					else
					{
						*found = FALSE;
					}
					return S_OK;
				}

				// write methods
				virtual HRESULT STDMETHODCALLTYPE SetAt(unsigned index, T item)
				{
					if (index < _v->size())
					{
						(*_v)[index] = item;
					}
					return S_OK;
				}

				virtual HRESULT STDMETHODCALLTYPE InsertAt(unsigned index, T item)
				{
					_v->emplace(_v->begin() + index, item);
					return S_OK;
				}

				virtual HRESULT STDMETHODCALLTYPE RemoveAt(unsigned index)
				{
					if (index < _v->size())
					{
						_v->erase(_v->begin() + index);
					}
					return S_OK;
				}

				virtual HRESULT STDMETHODCALLTYPE Append(T item)
				{
					_v->emplace_back(item);
					return S_OK;
				}

				virtual HRESULT STDMETHODCALLTYPE RemoveAtEnd()
				{
					if (!_v->empty())
					{
						_v->pop_back();
					}
					return S_OK;
				}

				virtual HRESULT STDMETHODCALLTYPE Clear()
				{
					_v->clear();
					return S_OK;
				}

				virtual HRESULT STDMETHODCALLTYPE First(ABI::Windows::Foundation::Collections::IIterator<T> **first)
				{
					Microsoft::WRL::ComPtr<Iterator<T>> p = Microsoft::WRL::Make<Iterator<T>>();
					p->Init(_v);
					*first = p.Detach();
					return S_OK;
				}

			private:
				std::shared_ptr<std::vector<T>> _v;
			};
		}
	}
}
