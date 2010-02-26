
#ifndef PROXYPART_H
#define PROXYPART_H


class Part;

class ProxyPart {
	
public:
	ProxyPart(Part* ptrActualPart){
		m_ptrActualPart = ptrActualPart;
		m_ptrNextProxyPart = NULL;
	}

	ProxyPart* m_ptrNextProxyPart;
	Part* m_ptrActualPart;
};

#endif