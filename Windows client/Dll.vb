Imports System.Runtime.InteropServices
Imports Estegaudio
Imports System.Security.Cryptography

Module Dll
    Public dir As String = IO.Directory.GetCurrentDirectory + "/"
    ' Direcciones: 85.136.5.97 '127.0.0.1:8080 '192.168.1.135

    Public nombre As String = ""
    Public ip As String = ""
    Public puerto As String = ""

    Public Function hashHex(cadena As String) As String
        Dim sha As System.Security.Cryptography.SHA1 = System.Security.Cryptography.SHA1.Create()
        Dim s As String = ""
        Dim hash As Byte() = sha.ComputeHash(System.Text.Encoding.ASCII.GetBytes(cadena))
        Dim c As Byte = 0
        While (c < 20)
            s += Conversion.Hex(hash(c))
            c += 1
        End While
        Return s
    End Function

    Public Function FormatFileSize(ByVal FileSizeBytes As Long) As String
        Dim sizeTypes() As String = {"B", "KB", "MB", "GB"}
        Dim Len As Decimal = FileSizeBytes
        Dim sizeType As Integer = 0
        Do While Len > 1024
            Len = Decimal.Round(Len / 1024, 2)
            sizeType += 1
            If sizeType >= sizeTypes.Length - 1 Then Exit Do
        Loop

        Dim Resp As String = Len.ToString & " " & sizeTypes(sizeType)
        Return Resp
    End Function

    Class Estegaudio
        <DllImport("Estegaudio.dll", CallingConvention:=CallingConvention.Cdecl, EntryPoint:="?introducir@Estegaudio@@YAHPAD00@Z")> _
        Public Shared Function introducir(archivo As String, sonido As String, destino As String) As Integer
        End Function

        <DllImport("Estegaudio.dll", CallingConvention:=CallingConvention.Cdecl, EntryPoint:="?extraer@Estegaudio@@YAHPAD0@Z")> _
        Public Shared Function extraer(sonido As String, destino As String) As Integer
        End Function

        Public Shared Function calcularS(tam As Long) As Integer
            Return Math.Round(((4 + tam) * 8 + 32) / 22000)
        End Function
    End Class

    Class Sonido
        <DllImport("Estegaudio.dll", CallingConvention:=CallingConvention.Cdecl, EntryPoint:="?grabarMensaje@Sonido@@YAHPADD@Z")> _
        Public Shared Function grabar(archivo As String, grabando As Byte) As Integer
        End Function


    End Class

    Class Encrypt

        Public Shared Sub desencriptar(origen As String, destino As String, clave As Byte())
            Shell("aescrypt -d -p """ + System.Text.Encoding.ASCII.GetString(clave) + """" + " -o """ + destino + """ """ + origen + """", AppWinStyle.Hide, True)
            'Dim r As Rijndael = Rijndael.Create()
            'r.BlockSize = 256
            'r.Key = clave
            'r.IV = clave

            'Dim decryptor As ICryptoTransform = r.CreateDecryptor(clave, clave)
            'Dim bytes(200000) As Byte

            '' Create the streams used for decryption.
            'Using msDecrypt As New IO.FileStream(origen, IO.FileMode.Open, IO.FileAccess.Read)

            '    Using csDecrypt As New CryptoStream(msDecrypt, decryptor, CryptoStreamMode.Read)

            '        ' Read the decrypted bytes from the decrypting stream
            '        ' and place them in a string.
            '        csDecrypt.Read(bytes, 0, bytes.Length)
            '        'srDecrypt.ReadBlock(bytes, 0, Integer.MaxValue)
            '        'bytes = System.Text.Encoding.ASCII.GetBytes(s)
            '        My.Computer.FileSystem.WriteAllBytes("modo1", bytes, False)
            '        'My.Computer.FileSystem.WriteAllText("modo2", s, False)
            '    End Using
            'End Using

            ''Create the file stream to read the encrypted file back.
            'Dim fsread As New IO.FileStream(origen, IO.FileMode.Open, IO.FileAccess.Read)
            ''Create the crypto stream set to read and to do a DES decryption transform on incoming bytes.
            'Dim cryptostreamDecr As New CryptoStream(fsread, decryptor, CryptoStreamMode.Read)
            ''Print out the contents of the decrypted file.
            'Dim fsDecrypted As New IO.StreamWriter(destino)
            'fsDecrypted.Write(New IO.StreamReader(cryptostreamDecr).ReadToEnd)
            'fsDecrypted.Flush()
            'fsDecrypted.Close()
        End Sub

        Public Shared Sub encriptar(origen As String, destino As String, clave As Byte())
            Shell("aescrypt -e -p """ + System.Text.Encoding.ASCII.GetString(clave) + """" + " -o """ + destino + """ """ + origen + """", AppWinStyle.Hide, True)
            'Dim r As Rijndael = Rijndael.Create()
            'r.BlockSize = 256
            'r.Key = clave
            'r.IV = clave

            'Dim encryptor As ICryptoTransform = r.CreateEncryptor(clave, clave)

            'Dim fsInput As New IO.FileStream(origen, IO.FileMode.Open, IO.FileAccess.Read)
            'Dim fsEncrypted As New IO.FileStream(destino, IO.FileMode.Create, IO.FileAccess.Write)

            'Dim csEncrypt As New CryptoStream(fsEncrypted, encryptor, CryptoStreamMode.Write)

            'Dim bytearrayinput(fsInput.Length - 1) As Byte
            'fsInput.Read(bytearrayinput, 0, bytearrayinput.Length)
            'csEncrypt.Write(bytearrayinput, 0, bytearrayinput.Length)
            'csEncrypt.Close()

        End Sub
    End Class

    Class Red 'Wrapper para la clase Red de la DLL
        Dim s_enviar As UInteger
        Dim s_recibir As UInteger
        Dim usuario As String 'Hash del usuario
        Dim ip As String
        Dim puerto As UInt16

        Public Sub New(_usuario As String, _ip As String, Optional _puerto As UInt16 = 8888)
            usuario = _usuario
            ip = _ip
            puerto = _puerto
            's_recibir = _configurarSocket(ip, puerto)
        End Sub

        Public Sub enviarMensaje(archivo As String, _ip As String, mensaje As String, _puerto As UInt16)
            Dim cmd As String = "red.exe e """ + archivo + """ " + _ip + " " + _puerto.ToString + " " + mensaje + " " + usuario
            '("red.exe e """ + archivo + """ " + _ip + " " + _puerto + " " + mensaje + " " + usuario)
            Shell(cmd, AppWinStyle.Hide, True, 60000)
            'Process.Start("red.exe", cmd).WaitForExit(60000)
        End Sub

        Public Sub recibeMensajes()
            Dim cmd As String = "r " + ip + " " + puerto.ToString
            Shell("red.exe " + cmd, AppWinStyle.Hide)
            'Chats.pRecibir = Process.Start("red.exe", cmd)
            'Chats.pRecibir()
        End Sub

        Public Function enviarMensaje2(archivo As String, _ip As String, mensaje As String, _puerto As UInt16) As Byte
            Return _enviarMensaje(s_recibir, _ip, usuario, archivo, mensaje, _puerto)
        End Function

        Public Sub recibeMensajes2(funcion As Chats.onMensajeCallback)
            _recibeMensajes(s_recibir, Dll.puerto, funcion)
        End Sub

        Public Sub dispose()
            _dispose(s_recibir)
        End Sub

        <DllImport("Estegaudio.dll", CallingConvention:=CallingConvention.Cdecl, EntryPoint:="?configurarSocket@Red@@YAIPADG@Z")> _
        Private Shared Function _configurarSocket(<MarshalAs(UnmanagedType.LPStr)> ip As String, puerto As UInt16) As UInteger
        End Function

        <DllImport("Estegaudio.dll", CallingConvention:=CallingConvention.Cdecl, EntryPoint:="?enviaMensaje@Red@@YADIPAD000G@Z")> _
        Private Shared Function _enviarMensaje(s As UInteger, <MarshalAs(UnmanagedType.LPStr)> ip As String, <MarshalAs(UnmanagedType.LPStr)> usuario As String, <MarshalAs(UnmanagedType.LPStr)> archivo As String, <MarshalAs(UnmanagedType.LPStr)> mensaje As String, puerto As UInt16) As Byte
        End Function

        <DllImport("Estegaudio.dll", CallingConvention:=CallingConvention.Cdecl, EntryPoint:="?recibeMensajes@Red@@YAXIP6AXPADH0@Z@Z")> _
        Private Shared Sub _recibeMensajes(s As UInteger, puerto As UInt16, funcion As Chats.onMensajeCallback)
        End Sub

        <DllImport("Estegaudio.dll", CallingConvention:=CallingConvention.Cdecl, EntryPoint:="?dispose@Red@@YAXI@Z")> _
        Private Shared Sub _dispose(s As UInteger)
        End Sub

        Private Shared Sub GetExternalIp()
            Try
                'Usando servidor remoto
                'ip = (New Net.WebClient()).DownloadString("http://checkip.dyndns.org/")
                'ip = (New System.Text.RegularExpressions.Regex("\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}")) _
                '.Matches(ip)(0).ToString()
                'Usando servidor propio
                Dll.ip = (New Net.WebClient()).DownloadString(My.Settings.servidor + "?tipo=ip")
            Catch
                MsgBox("No se pudo obtener la IP externa.")
                End
            End Try
        End Sub

        Public Shared Function iniciarSesion(nombre As String) As Boolean
            Try
                GetExternalIp()
                If (New Net.WebClient()).DownloadString(My.Settings.servidor + "?tipo=registro&id=" + nombre + "&ip=" + Dll.ip + "&port=" + Dll.puerto) = "OK" Then
                    Dll.nombre = nombre
                    Return True
                Else
                    Return False
                End If
            Catch ex As Exception
                Return False
            End Try
        End Function

        Public Shared Sub cerrarSesion()
            Try
                Dim res As String = (New Net.WebClient()).DownloadString(My.Settings.servidor + "?tipo=logoff&id=" + nombre + "&ip=" + Dll.ip + "&port=" + Dll.puerto)
            Catch ex As Exception
                MsgBox(ex.ToString())
                End
            End Try
        End Sub

        Public Shared Function buscarUsuario(nombre As String) As String
            Try
                Dim res As String = (New Net.WebClient()).DownloadString(My.Settings.servidor + "?tipo=req&id=" + nombre)
                If res = "Not found" Then
                    Return ""
                Else
                    Return res
                End If
            Catch ex As Exception
                Return ""
            End Try
        End Function

    End Class

    Class Codec
        <DllImport("Estegaudio.dll", CallingConvention:=CallingConvention.Cdecl, EntryPoint:="?codifica@Codec@@YAXPAD0@Z")> _
        Public Shared Sub encode(entrada As String, salida As String)
        End Sub

        <DllImport("Estegaudio.dll", CallingConvention:=CallingConvention.Cdecl, EntryPoint:="?decodifica@Codec@@YAXPAD0@Z")> _
        Public Shared Sub decode(entrada As String, salida As String)
        End Sub

        <DllImport("Estegaudio.dll", CallingConvention:=CallingConvention.Cdecl, EntryPoint:="?writeWaveFile2@Sonido@@YAXPBDPAD@Z")> _
        Public Shared Sub writeWavFile(origen As String, destino As String)
        End Sub

        Public Shared Function encode2(entrada As String, salida As String) As Boolean
            Return Shell("flac.exe -0 """ + entrada + """ -o """ + salida + """ -f", AppWinStyle.Hide, True)
            Return Not My.Computer.FileSystem.FileExists(salida)
        End Function

        Public Shared Function decode2(entrada As String, salida As String) As Boolean
            Shell("flac.exe -d """ + entrada + """ -o """ + salida + """ -f --force-raw-format --endian=little --sign=unsigned", AppWinStyle.Hide, True)
            Return Not My.Computer.FileSystem.FileExists(salida)
        End Function
    End Class
End Module
