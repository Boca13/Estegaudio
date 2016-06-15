
Public Class Chat

    Public usuario As String
    Public hashUsuario As String

    Dim ip As String
    Dim puerto As UInt16

    Dim grabando As Byte = 0
    Dim hilo As Threading.Thread
    Friend WithEvents contraseña As System.Windows.Forms.ToolStripTextBox = New System.Windows.Forms.ToolStripTextBox
    Friend WithEvents generarContraseña As System.Windows.Forms.ToolStripButton = New System.Windows.Forms.ToolStripButton("Generar")
    Friend WithEvents verContraseña As System.Windows.Forms.ToolStripButton = New System.Windows.Forms.ToolStripButton("Ver")
    Dim tiempo As Date
    Dim sha As System.Security.Cryptography.SHA1 = System.Security.Cryptography.SHA1.Create()

    Dim siguiente As Integer = 5

    Sub New(_usuario As String, _ip As String, _puerto As UInt16)
        InitializeComponent()
        usuario = _usuario
        Me.ip = _ip
        Me.puerto = _puerto
        Me.Text = "Estegaudio - " + usuario
        hashUsuario = hashHex(usuario)
    End Sub

    Friend Sub onMensaje(mensaje As String)
        Try
            If (Dll.Codec.decode2("mensajes/" + mensaje + ".flac", "mensajes/" + mensaje)) Then Throw New FormatException
            My.Computer.FileSystem.DeleteFile(dir + "mensajes/" + mensaje + ".flac")
            Dll.Codec.writeWavFile("mensajes/" + mensaje, "mensajes/" + mensaje + ".wav")
            Dim m As Mensaje = New Mensaje(mensaje, Me)
            m.Location = New Point(5, siguiente)
            m.BackColor = Color.PaleTurquoise
            Panel1.Controls.Add(m)
            NudgeMe()

            'm.Anchor = AnchorStyles.None 'Left And AnchorStyles.Top
            'SplitContainer1.Panel1.AutoScrollMinSize = SplitContainer1.Panel1.ClientSize
            siguiente += 70
        Catch ex As Exception
            MsgBox(ex.Message, MsgBoxStyle.Critical, ex.InnerException)
        End Try
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        If grabando = 1 Then 'Parar de grabar
            grabando = 0
            Button1.Text = "Grabar"


            Dim id() As Byte = sha.ComputeHash(System.Text.Encoding.ASCII.GetBytes((My.Computer.Clock.GmtTime.Subtract(New Date(0)).TotalMilliseconds.ToString)))
            Dim ids As String = ""
            Dim c As Byte = 0
            While (c < 20)
                ids += Conversion.Hex(id(c))
                c += 1
            End While

            Try
                Sonido.grabar(dir + "mensajes/" + ids + ".wav", 0)
                Timer1.Stop()
                If (TextBox2.Text <> "") Then
                    Dll.Encrypt.encriptar(TextBox2.Text, dir + "mensajes/" + ids + ".aes", System.Text.Encoding.ASCII.GetBytes(contraseña.Text))
                    Dll.Estegaudio.introducir(dir + "mensajes/" + ids + ".aes", dir + "mensajes/" + ids + ".wav", dir + "mensajes/" + ids + "2.wav")
                Else
                    Dll.Estegaudio.introducir("", dir + "mensajes/" + ids + ".wav", dir + "mensajes/" + ids + "2.wav")
                End If
                If (My.Computer.FileSystem.GetFileInfo(dir + "mensajes/" + ids + "2.wav").Length = 0) Then Throw New IO.FileLoadException
                If (Dll.Codec.encode2(dir + "mensajes/" + ids + "2.wav", dir + "mensajes/" + ids + ".flac")) Then Throw New FormatException
                Chats.red.enviarMensaje(dir + "mensajes/" + ids + ".flac", Me.ip, ids, Me.puerto)
            Catch ex As Exception
                MsgBox(ex.Message, MsgBoxStyle.Critical)
                Try
                    My.Computer.FileSystem.DeleteFile(dir + "mensajes/" + ids + ".wav")
                    My.Computer.FileSystem.DeleteFile(dir + "mensajes/" + ids + "2.wav")
                    My.Computer.FileSystem.DeleteFile(dir + "mensajes/" + ids + ".flac")
                    My.Computer.FileSystem.DeleteFile(dir + "mensajes/" + ids + ".aes")
                Catch ex1 As Exception
                End Try
            Finally
                Try
                    My.Computer.FileSystem.DeleteFile(dir + "mensajes/" + ids + ".wav")
                    My.Computer.FileSystem.RenameFile(dir + "mensajes/" + ids + "2.wav", ids + ".wav")
                    My.Computer.FileSystem.DeleteFile(dir + "mensajes/" + ids + ".flac")
                    My.Computer.FileSystem.DeleteFile(dir + "mensajes/" + ids + ".aes")
                Catch ex As Exception
                End Try

                Dim m As Mensaje = New Mensaje(ids, Me)
                m.BackColor = Color.LightGreen
                m.Location = New Point(205, siguiente)
                Panel1.Controls.Add(m)
                'm.Anchor = AnchorStyles.Right And AnchorStyles.Top

                'SplitContainer1.Panel1.AutoScrollMinSize = SplitContainer1.Panel1.ClientSize
                siguiente += 70
            End Try
            Label4.Text = ""
            TextBox2.Text = ""
        Else 'Empezar a grabar
            grabando = 1
            Button1.Text = "Parar"
            Sonido.grabar("", 1)
            tiempo = My.Computer.Clock.GmtTime
            Label4.ForeColor = Color.Red
            Timer1.Start()
        End If

    End Sub

    Private Sub Chat_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        Try
            If (My.Settings.fondo <> "") Then SplitContainer1.Panel1.BackgroundImage = Image.FromFile(My.Settings.fondo)
        Catch ex As Exception
            MsgBox("Error al cargar imagen de fondo: " + ex.Message)
            My.Settings.fondo = ""
        End Try
        Dim tb As TextBox = contraseña.Control
        tb.UseSystemPasswordChar = True
        StatusStrip1.Items.Add(contraseña)
        StatusStrip1.Items.Add(verContraseña)
        StatusStrip1.Items.Add(generarContraseña)
    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        If (OpenFileDialog1.ShowDialog() = Windows.Forms.DialogResult.OK) Then
            TextBox2.Text = OpenFileDialog1.FileName
        End If
    End Sub

    Private Sub TextBox2_TextChanged(sender As Object, e As EventArgs) Handles TextBox2.TextChanged
        Try
            If (TextBox2.Text <> "") Then
                Dim f As IO.FileInfo = My.Computer.FileSystem.GetFileInfo(TextBox2.Text)
                Label2.Text = Dll.FormatFileSize(f.Length)
                Dim necesarios As Integer = Dll.Estegaudio.calcularS(f.Length)
                Label3.Text = "Necesarios " + necesarios.ToString + " s"
                Label3.Tag = necesarios
            Else
                Label3.Text = ""
                Label2.Text = ""
            End If
        Catch ex As Exception
            MsgBox(ex.ToString, MsgBoxStyle.Exclamation)
            Label2.Text = ""
            Label3.Text = ""
        End Try
    End Sub

    Private Sub Timer1_Tick(sender As Object, e As EventArgs) Handles Timer1.Tick
        Label4.Text = (Convert.ToInt32(My.Computer.Clock.GmtTime.Subtract(tiempo).TotalSeconds)).ToString + " s"
        If Label4.Text.Substring(0, Label4.Text.Length - 2) > Label3.Tag Then
            Label4.ForeColor = Color.Lime
        End If
    End Sub

    Sub validarPass() Handles contraseña.LostFocus
        If contraseña.Text.Length <> 32 Then
            MsgBox("La contraseña debe tener 32 caracteres")
        End If
    End Sub

    Sub generarContraseña_Click() Handles generarContraseña.Click
        Dim c As Byte = 0
        Dim r As Random = New Random()
        Dim b(31) As Byte
        r.NextBytes(b)
        Randomize()
        While (c < 32)
            b(c) = (b(c) * ((126 - 35) / 256) + 35)
            c += 1
        End While
        contraseña.Text = System.Text.Encoding.ASCII.GetString(b)
    End Sub

    Sub verContraseña_Click() Handles verContraseña.Click
        Dim tb As TextBox = contraseña.Control
        tb.UseSystemPasswordChar = Not tb.UseSystemPasswordChar
    End Sub

    Private Sub Chat_FormClosing(sender As Object, e As FormClosingEventArgs) Handles MyBase.FormClosing
        Chats.chats.Remove(Me)
        For Each item As ToolStripDropDownItem In Chats.AbrirChatToolStripMenuItem.DropDownItems
            If (item.Text = Me.usuario) Then
                Chats.AbrirChatToolStripMenuItem.DropDownItems.Remove(item)
                Exit For
            End If
        Next
    End Sub

    Friend Sub mostrar()
        Me.Show()
        Me.BringToFront()
        Me.WindowState = FormWindowState.Normal
        Me.Focus()
    End Sub

    Private Sub Panel1_ControlAdded(sender As Object, e As ControlEventArgs) Handles Panel1.ControlAdded
        SplitContainer1.Panel1.ScrollControlIntoView(Control1)
    End Sub

    Public Sub NudgeMe()
        ' Store the original location of the form.
        Dim xCoord As Integer = Me.Location.X
        Dim yCoord As Integer = Me.Location.Y

        ' An integer for storing the random number each time
        Dim rnd As Integer = 0

        'Instantiate the random generation mechanism
        Dim RandomClass As Random = New Random()
        Dim i As Integer = 0
        While (i <= 1000)
            Me.Location = New Point(RandomClass.Next(xCoord + 1, xCoord + 15), RandomClass.Next(yCoord + 1, yCoord + 15))
            i += 1
        End While

        'Restore the original location of the form
        Me.Location = New Point(xCoord, yCoord)

    End Sub

    Private Sub CambiarFondoToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles CambiarFondoToolStripMenuItem.Click
        If (OpenFileDialog2.ShowDialog() = Windows.Forms.DialogResult.OK) Then
            My.Settings.fondo = OpenFileDialog2.FileName
            Try
                SplitContainer1.Panel1.BackgroundImage = Image.FromFile(My.Settings.fondo)
            Catch ex As Exception
                MsgBox(ex.Message)
            End Try
        End If
    End Sub

    Private Sub QuitarFondoToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles QuitarFondoToolStripMenuItem.Click
        SplitContainer1.Panel1.BackgroundImage = Nothing
    End Sub
End Class