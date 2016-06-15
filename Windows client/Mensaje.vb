Imports System.Media

Public Class Mensaje

    Public Property id As String = ""

    Private hash(19) As Byte
    Private sonido As SoundPlayer
    Private reproduciendo As Boolean = False

    Dim padre As Chat

    Public Sub New(mensaje As String, ByRef _padre As Chat)
        ' Llamada necesaria para el diseñador.
        InitializeComponent()
        id = mensaje
        padre = _padre
        sonido = New SoundPlayer(dir + "mensajes/" + mensaje + ".wav")
        Try
            Label1.Text = Math.Round((My.Computer.FileSystem.GetFileInfo(dir + "mensajes/" + id + ".wav").Length - 64) / 22000)
            Label1.Tag = Convert.ToInt32(Math.Round((My.Computer.FileSystem.GetFileInfo(dir + "mensajes/" + id + ".wav").Length - 64) / 22.0))
        Catch ex As Exception
        End Try
    End Sub

    Public Sub New(mhash As Byte(), ByRef _padre As Chat)
        ' Llamada necesaria para el diseñador.
        InitializeComponent()
        mhash.CopyTo(hash, 0)
        padre = _padre
        Dim c As Byte = 0
        While (c < 20)
            id += Conversion.Hex(hash(c))
            c += 1
        End While
        sonido = New SoundPlayer(dir + "mensajes/" + id + ".wav")
        Try
            Label1.Text = Math.Round((My.Computer.FileSystem.GetFileInfo(dir + "mensajes/" + id + ".wav").Length - 64) / 22000)
            Label1.Tag = (My.Computer.FileSystem.GetFileInfo(dir + "mensajes/" + id + ".wav").Length - 64) / 22.0
        Catch ex As Exception
        End Try
    End Sub

    Private Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click
        If reproduciendo Then
            sonido.Stop()
            Timer2.Stop()
            Timer1.Stop()
            Label2.Text = "0"
            Button3.BackgroundImage = My.Resources.play
            reproduciendo = False
            PictureBox1.Visible = False
        Else
            sonido.Play()
            Button3.BackgroundImage = My.Resources.pause
            reproduciendo = True
            PictureBox1.Visible = True
            If (Label1.Tag = 0) Then
                Timer1.Interval = 600
            Else
                Timer1.Interval = Label1.Tag
            End If

        Timer1.Start()
        Timer2.Start()
        End If
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click

        If (SaveFileDialog1.ShowDialog() = DialogResult.OK) Then
            Dim s As String = SaveFileDialog1.FileName
            If (s <> "") Then
                If (Dll.Estegaudio.extraer(dir + "mensajes/" + id + ".wav", dir + "mensajes/" + id + ".aes") > 0) Then
                    Dll.Encrypt.desencriptar(dir + "mensajes/" + id + ".aes", s, System.Text.Encoding.ASCII.GetBytes(padre.contraseña.Text))
                    Dim info As IO.FileInfo = My.Computer.FileSystem.GetFileInfo(s)
                    If (info.Exists) Then
                        If (info.Length > 0) Then
                            MsgBox("Guardado archivo con " + Dll.FormatFileSize(info.Length), MsgBoxStyle.Information, "Archivo extraído")
                            My.Computer.FileSystem.DeleteFile(dir + "mensajes/" + id + ".aes")
                        Else
                            MsgBox("No se pudo desencriptar el archivo.", MsgBoxStyle.Exclamation)
                        End If
                    Else
                        MsgBox("No se pudo extraer ningún archivo.", MsgBoxStyle.Exclamation)
                    End If
                Else
                    MsgBox("No se pudo extraer ningún archivo.", MsgBoxStyle.Exclamation)
                End If
            End If
        End If
    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        My.Computer.FileSystem.DeleteFile(dir + "mensajes/" + id + ".wav")
        Try
            My.Computer.FileSystem.DeleteFile(dir + "mensajes/" + id)
        Catch ex As Exception
        End Try
        Me.Dispose()
    End Sub

    Private Sub Timer1_Tick(sender As Object, e As EventArgs) Handles Timer1.Tick
        sonido.Stop()
        Button3.BackgroundImage = My.Resources.play
        reproduciendo = False
        PictureBox1.Visible = False
        Timer1.Stop()
        Timer2.Stop()
        Label2.Text = "0"
    End Sub

    Private Sub Timer2_Tick(sender As Object, e As EventArgs) Handles Timer2.Tick
        Label2.Text = Label2.Text + 1
    End Sub
End Class
