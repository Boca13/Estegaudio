<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
	pageEncoding="ISO-8859-1"%>
<%@ page trimDirectiveWhitespaces="true"%>
<%@ page import="Estegaudio.*"%>
<%! static Clientes clientes = new Clientes();%>
<%
	String tipo = request.getParameter("tipo");
	if (tipo != null) {
		if (tipo.equals("registro")) {
			String id = request.getParameter("id");
			String ip = request.getParameter("ip");
			String port = request.getParameter("port");
			if ((!id.isEmpty()) && (!ip.isEmpty()) && (!port.isEmpty())) {
				if (!clientes.existe(id)) {
					clientes.clientes.add(new Cliente(id, ip, port));
					out.print("OK");
				} else {
					if (clientes.existe(id, ip, port)) //Si es él mismo
						out.print("OK");
					else
						out.print("error: ya existe un usuario con ese nombre");
				}
			} else
				out.print("error");
		} else if (tipo.equals("ip")) {
			out.print(request.getRemoteAddr());
		} else if (tipo.equals("logoff")) {
			String id = request.getParameter("id");
			String ip = request.getParameter("ip");
			String port = request.getParameter("port");
			if ((!id.isEmpty()) && (!ip.isEmpty()) && (!port.isEmpty())) {
				if (clientes.existe(id, ip, port))
					clientes.clientes.remove(clientes.buscar(id));
				else
					out.print("error");
			} else
				out.print("error");
		} else if (tipo.equals("req")) {
			String id = request.getParameter("id");
			if (!id.isEmpty()) {
				if (clientes.buscar(id) >= 0) {
					Cliente encontrado = clientes.clientes.get(clientes
							.buscar(id));
					if (encontrado != null) {
						out.print(encontrado.getId() + ";"
								+ encontrado.getIp() + ";"
								+ encontrado.getPort());
					} else
						out.print("Not found");
				} else
					out.print("Not found");
			} else
				out.print("error");
		}
	} else {
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Servidor de Presencia: Estegaudio</title>
</head>
<body>
	<script type="text/javascript">
		window.location.replace("index.html")
	</script>
	<noscript>
		<a href="index.html">Ir a página principal</a>
	</noscript>
</body>
</html>
<%
	}
%>