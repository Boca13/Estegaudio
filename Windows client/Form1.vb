Public Class Form1

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        If TextBox1.Text <> "" Then
            Me.Cursor = Cursors.WaitCursor
            Dll.puerto = NumericUpDown1.Value
            If (Red.iniciarSesion(TextBox1.Text)) Then
                Chats.Label1.Text = nombre
                Chats.red = New Dll.Red(Dll.hashHex(nombre), ip, NumericUpDown1.Value)
                Me.DialogResult = Windows.Forms.DialogResult.OK
                Me.Close()
            Else
                MsgBox("No se pudo iniciar sesión.", MsgBoxStyle.Critical)
            End If
            Me.Cursor = Cursors.Default
        End If
    End Sub

    Private Sub Form1_FormClosed(sender As Object, e As FormClosedEventArgs) Handles MyBase.FormClosed
        My.Settings.usuario = TextBox1.Text
        My.Settings.Save()
    End Sub

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        TextBox1.Text = My.Settings.usuario
        TextBox1.SelectAll()
    End Sub
End Class
