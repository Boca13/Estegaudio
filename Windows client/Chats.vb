Public Class Chats
    Public chats As List(Of Chat) = New List(Of Chat)
    Friend red As Dll.Red
    Friend ultimo As Chat

    Private Sub Chats_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        If (Form1.ShowDialog() <> Windows.Forms.DialogResult.OK) Then End

        My.Application.SaveMySettingsOnExit = True
        If Not (IsNothing(My.Settings.Nombres)) Then
            Dim c As Int16 = 0
            While (c < My.Settings.Nombres.Count)
                ListView1.Items.Add(My.Settings.Nombres(c))
                NuevoChatToolStripMenuItem.DropDownItems.Add(My.Settings.Nombres(c), Nothing, AddressOf NuevoChatToolStripMenuItem_Click)
                c += 1
            End While
            actualizarListView()
            red.recibeMensajes()

        End If
        Try
            My.Computer.FileSystem.DeleteDirectory("mensajes", FileIO.DeleteDirectoryOption.DeleteAllContents)
            My.Computer.FileSystem.CreateDirectory("mensajes")
        Catch ex As Exception
            Try
                My.Computer.FileSystem.CreateDirectory("mensajes")
            Catch ex2 As Exception
            End Try
        End Try
        NotifyIcon1.Visible = True
        Timer2.Start()
    End Sub

    Public Delegate Sub onMensajeCallback(mensaje As String, n As Integer, usuario As String)

    Sub onMensaje(mensaje As String, n As Integer, usuario As String)
        Dim chat As Chat = Nothing

        For Each item As Chat In chats
            If (item.hashUsuario = usuario) Then
                chat = item
                NotifyIcon1.ShowBalloonTip(5000, item.usuario, "Recibido mensaje de " + item.usuario, ToolTipIcon.Info)
                ultimo = chat
                Exit For
            End If
        Next
        'Si se quisiera abrir conversación con un desconocido. Harían falta más datos desde Red: ip y puerto del usuario anónimo que conecta.
        'If (IsNothing(chat)) Then
        '    chat = New Chat(usuario, )
        '    chat.Show()
        'End If

        Try
            chat.onMensaje(mensaje)
        Catch ex As Exception
        End Try


    End Sub

    Private Sub mostrar(sender As Object, e As EventArgs)
        Dim s As ToolStripDropDownItem = sender
        Dim c As Chat = s.Tag
        c.mostrar()
    End Sub

    Private Sub ListView1_DoubleClick(sender As Object, e As EventArgs) Handles ListView1.DoubleClick, AbrirConversaciónToolStripMenuItem.Click
        If (ListView1.SelectedItems.Count = 1) Then
            If (ListView1.SelectedItems(0).ImageIndex = 1) Then
                Dim tag As String = ListView1.SelectedItems(0).Tag
                Dim c As Chat = New Chat(ListView1.SelectedItems(0).Text, tag.Substring(tag.IndexOf(";") + 1, tag.LastIndexOf(";") - tag.IndexOf(";") - 1), tag.Substring(tag.LastIndexOf(";") + 1))
                chats.Add(c)
                AbrirChatToolStripMenuItem.DropDownItems.Add(c.usuario, Nothing, AddressOf mostrar).Tag = c
                c.Show()
            End If
        End If
    End Sub

    Private Sub NuevoChatToolStripMenuItem_Click(sender As Object, e As EventArgs)
        Dim s As ToolStripDropDownItem = sender
        Dim elemento As ListViewItem = Nothing
        For Each i As ListViewItem In ListView1.Items
            If (i.Text = s.Text) Then
                elemento = i
                Exit For
            End If
        Next

        If Not (IsNothing(elemento)) Then
            If (elemento.ImageIndex = 1) Then
                Dim tag As String = elemento.Tag
                Dim c As Chat = New Chat(elemento.Text, tag.Substring(tag.IndexOf(";") + 1, tag.LastIndexOf(";") - tag.IndexOf(";") - 1), tag.Substring(tag.LastIndexOf(";") + 1))
                chats.Add(c)
                AbrirChatToolStripMenuItem.DropDownItems.Add(c.usuario, Nothing, AddressOf mostrar).Tag = c
                c.Show()
            End If
        End If
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        Dim usuario As String
        usuario = InputBox("Introduzca nombre de usuario:", "Añadir un amigo")
        If usuario <> "" Then
            Dim i As ListViewItem = ListView1.Items.Add(usuario)
            i.Tag = red.buscarUsuario(usuario)
            NuevoChatToolStripMenuItem.DropDownItems.Add(usuario, Nothing, AddressOf NuevoChatToolStripMenuItem_Click)
            If (i.Tag <> "") Then
                i.ImageIndex = 1
            Else
                i.ImageIndex = 0
            End If
        End If
    End Sub

    Private Sub actualizarListView() Handles Timer1.Tick, ActualizarToolStripMenuItem.Click
        Dim c As Int16 = 0
        While (c < ListView1.Items.Count)
            ListView1.Items(c).Tag = red.buscarUsuario(ListView1.Items(c).Text)
            If (ListView1.Items(c).Tag <> "") Then
                ListView1.Items(c).ImageIndex = 1
            Else
                ListView1.Items(c).ImageIndex = 0
            End If
            c += 1
        End While
    End Sub

    Private Sub Chats_FormClosed() Handles MyBase.FormClosed, SalirToolStripMenuItem.Click
        If Not (IsNothing(My.Settings.Nombres)) Then
            My.Settings.Nombres.Clear()
        Else
            My.Settings.Nombres = New ArrayList()
        End If

        Dim c As Int16 = 0
        While (c < ListView1.Items.Count)
            My.Settings.Nombres.Add(ListView1.Items(c).Text)
            c += 1
        End While
        My.Settings.Save()
        Try
            Dim pr As Process = Process.GetProcessesByName("red")(0)
            If Not (IsNothing(pr)) Then pr.Kill()
        Catch ex As Exception
        End Try

        NotifyIcon1.Dispose()
        'Cerrar sesión
        red.cerrarSesion()
        red.dispose()
        End
    End Sub

    Private Sub EliminarToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles EliminarToolStripMenuItem.Click
        If (ListView1.SelectedItems.Count = 1) Then
            ListView1.Items.Remove(ListView1.SelectedItems(0))
        End If
    End Sub

    Private Sub ContextMenuStrip1_Opening(sender As Object, e As System.ComponentModel.CancelEventArgs) Handles ContextMenuStrip1.Opening
        If (ListView1.SelectedItems.Count = 1) Then
            AbrirConversaciónToolStripMenuItem.Enabled = True
            EliminarToolStripMenuItem.Enabled = True
        Else
            AbrirConversaciónToolStripMenuItem.Enabled = False
            EliminarToolStripMenuItem.Enabled = False
        End If
    End Sub

    Private Sub Timer2_Tick(sender As Object, e As EventArgs) Handles Timer2.Tick
        If (My.Computer.FileSystem.FileExists("recibido") And My.Computer.FileSystem.FileExists("recibido_usuario")) Then
            Dim usuario As String = My.Computer.FileSystem.ReadAllText("recibido_usuario")
            My.Computer.FileSystem.DeleteFile("recibido_usuario")
            Dim msj As String = hashHex((My.Computer.Clock.GmtTime.Subtract(New Date(0)).TotalMilliseconds.ToString))
            My.Computer.FileSystem.RenameFile("recibido", msj)
            My.Computer.FileSystem.MoveFile(msj, "mensajes/" + msj + ".flac")
            onMensaje(msj, 0, usuario)
        End If
    End Sub

    Private Sub NotifyIcon1_MouseDoubleClick() Handles NotifyIcon1.DoubleClick, AbrirVentanaToolStripMenuItem.Click
        Me.Show()
    End Sub

    Private Sub NotifyIcon1_BalloonTipClicked(sender As Object, e As EventArgs) Handles NotifyIcon1.BalloonTipClicked
        If Not (IsNothing(ultimo)) Then
            ultimo.mostrar()
            ultimo = Nothing
        End If
    End Sub

    Private Sub Chats_Resize(sender As Object, e As EventArgs) Handles MyBase.Resize
        If WindowState = FormWindowState.Minimized Then
            Me.Hide()
        End If
    End Sub

    Private Sub NotifyIcon1_Click(sender As Object, e As EventArgs) Handles NotifyIcon1.Click
        ContextMenuStrip2.Show()
    End Sub
End Class