#pragma once
#include "pch.h"
#include <vector>

template <typename Clau, typename Valor, int size = 1000>
class Hash
{
public:
	Hash();
	~Hash();
	Valor& operator[](const Clau& clau);
	void insert(const Clau& clau, const Valor& valor);
	bool find(const Clau& clau, Valor& valor);
	bool esborra(const Clau& clau);

private:
	int codi(Clau c);
	int codi2(int index, int& vegades);
	void insertIntern(const Clau& clau, const Valor& valor);
	void rehash(int increase);

	std::vector<std::pair<Clau,Valor>> m_hash;
	std::vector<int> m_estat;
	int m_numOcupats;
	int m_maxElements;
	Valor m_valorDefecte;
	Clau m_clauDefecte;

	static const int LLIURE = 0;
	static const int OCUPAT = 1;
	static const int ESBORRAT = 2;
};


template <typename Clau, typename Valor, int size>
Hash<Clau,Valor, size>::Hash()
{
	m_maxElements = size;

	m_hash.resize(m_maxElements);
	m_estat.resize(m_maxElements, int(LLIURE));

	m_numOcupats = 0;
}

template <typename Clau, typename Valor, int size>
Hash<Clau,Valor, size>::~Hash()
{

}

template <typename Clau, typename Valor, int size>
void Hash<Clau,Valor, size>::insert(const Clau& clau, const Valor& valor)
{
	int index = codi(clau);
	if (m_hash[index].first == clau)
	{	
		m_hash[index].second = valor;
	}
	else
	{   
		int index2 = index, i = 0;
		while (i < m_maxElements)
		{
			if (m_hash[index2].first == clau)
			{
				m_hash[index2].second = valor;
				break;
			}
			if (m_estat[index2] == LLIURE)
			{
				insertIntern(clau, valor);
				break;
			}
			i++;
			index2 = codi2(index, i);
		}

	}
}

template <typename Clau, typename Valor, int size>
bool Hash<Clau,Valor, size>::find(const Clau& clau, Valor& valor)
{
	int index = codi(clau);

	if (m_hash[index].first == clau)
	{
		valor = m_hash[index].second;
		return true;
	}
	else
	{
		int index2 = index, i = 0;
		while (i < m_maxElements)
		{
			if (m_hash[index2].first == clau)
			{
				valor = m_hash[index2].second;
				return true;
			}
			if (m_estat[index2] == LLIURE)
			{
				break;
			}
			i++;
			index2 = codi2(index, i);
		}
		valor = m_valorDefecte;
		return false;
	}
}

template <typename Clau, typename Valor, int size>
int Hash<Clau,Valor, size>::codi(Clau s)
{
	unsigned long valor = 0;
	for (int i = 0; i < s.size(); i++)
	{
		valor += (s[i] * pow(2, i));
		valor = valor % m_maxElements;
	}
	return valor;
}

template <typename Clau, typename Valor, int size>
int Hash<Clau,Valor, size>::codi2(int index, int& vegades) 
{
	if (m_numOcupats >= m_maxElements)
	{
		rehash(1000); // no hi ha espai, ampliar
		vegades = 0;
	}

	return ((int)(index + vegades)) % m_maxElements;
}

template <typename Clau, typename Valor, int size>
void Hash<Clau,Valor, size>::insertIntern(const Clau& clau, const Valor& valor)
{
	int index = codi(clau);
	if (m_estat[index] != OCUPAT)
	{//Inserim element a posicio buida
		m_hash[index].first = clau;
		m_hash[index].second = valor;
		m_estat[index] = OCUPAT;
		m_numOcupats++;
	}
	else //Sino esta buida es COLLISIO
	{
		int index2, i = 1;
		while (i < m_maxElements)
		{
			index2 = codi2(index, i);
			if (m_estat[index2] != OCUPAT)
			{
				m_hash[index2].first = clau;
				m_hash[index2].second = valor;
				m_estat[index2] = OCUPAT;
				m_numOcupats++;
				break;
			}
			i++;
		}
	}

}

template <typename Clau, typename Valor, int size>
Valor& Hash<Clau,Valor, size>::operator[](const Clau& clau)
{
	int index = codi(clau);

	if (m_hash[index].first == clau)
	{
		//Element trobat
		return m_hash[index].second;
	}
	else
	{
		int index2 = index, i = 0;
		while (i < m_maxElements)
		{
			if (m_hash[index2].first == clau)
			{
				//Element trobat
				return m_hash[index2].second;
			}
			if (m_estat[index2] == LLIURE)
			{
				break;
			}
			i++;
			index2 = codi2(index, i);
		}

		return m_valorDefecte;
	}
}

template <typename Clau, typename Valor, int size>
bool Hash<Clau,Valor, size>::esborra(const Clau& clau)
{
	int index = codi(clau);

	if (m_hash[index].first == clau)
	{
		m_hash[index].first = m_clauDefecte;
		m_hash[index].second = m_valorDefecte;
		m_numOcupats--;
		m_estat[index] = ESBORRAT;
		return true;
	}
	else
	{
		int index2 = index, i = 0;
		while (i < m_maxElements)
		{
			if (m_hash[index2].first == clau)
			{
				//Element trobat
				m_hash[index2].first = m_clauDefecte;
				m_hash[index2].second = m_valorDefecte;
				m_numOcupats--;
				m_estat[index2] = ESBORRAT;
				return true;
			}
			if (m_estat[index2] == LLIURE)
			{
				break;
			}
			i++;
			index2 = codi2(index, i);
		}
		//Element no trobat
		return false;
	}
}

template <typename Clau, typename Valor, int size>
void Hash<Clau, Valor, size>::rehash(int increase)
{
	m_maxElements += increase;

	std::vector<std::pair<Clau, Valor>> aux1 = m_hash;
	std::vector<int> aux2 = m_estat;


	m_hash.resize(m_maxElements);
	m_estat.resize(m_maxElements, int(LLIURE));

	for (int i = 0; i < m_maxElements- increase; i++)
	{
		if (aux2[i] == OCUPAT)
		{
			insert(aux1[i].first, aux1[i].second);
		}
	}
}