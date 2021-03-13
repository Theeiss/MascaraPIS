#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	for( int iArq = 1; iArq < argc; ++iArq)
	{
		CString sFile = (char*) argv[iArq]; // O segundo argumento é o nome do arquivo.
		CFile file;

		file.Open(sFile, CFile::modeRead);

		file.Seek(0, CFile::begin);
		CString sDadosArq, sDadosArqNovo;

		file.Read(sDadosArq.GetBuffer(file.GetLength() + 1), file.GetLength());
		sDadosArq.ReleaseBuffer();

		file.Close();

		int iLinha = 0;
		CString sLinha = sDadosArq.Tokenize("\r\n", iLinha);

		while( sLinha.IsEmpty() == false )
		{
			if( sLinha.Find(";") > 0 )
			{
				int iCampo = 0;
				iCampo = sLinha.Find(";", 0 ) + 1;

				{
					//Máscara do PIS
					CString sPIS = sLinha.Left( sLinha.Find(";", iCampo) );

					sPIS = sPIS.Right( sPIS.GetLength() - iCampo );
					CString sPISFormatado = sPIS;

					sPISFormatado.Insert(3, ".");
					sPISFormatado.Insert(9, ".");
					sPISFormatado.Insert(12, ".");

					sLinha.Replace(sPIS, sPISFormatado);

					iCampo = sLinha.Find(";", iCampo ) + 1;
				}

				//Trata quarto campo negativo e terceiro campo troca de 035 para 023
				{
					CString sCampo3 = sLinha.Left( sLinha.Find(";", iCampo) );
					sCampo3 = sCampo3.Right(sCampo3.GetLength() - iCampo );

					iCampo = sLinha.Find(";", iCampo ) + 1;

					CString sCampo4 = sLinha.Left( sLinha.Find(";", iCampo) );
					sCampo4 = sCampo4.Right(sCampo4.GetLength() - iCampo);

					CString sAux3 = sCampo3;
					CString sAux4 = sCampo4;
					if( sAux4.Find("-") >= 0 )
					{
						sAux4.Replace("-", "0");
						sAux3= "023";

						sAux3 += ";" + sAux4 + ";";
						sAux4 = sCampo3 + ";" + sCampo4 + ";";

						sLinha.Replace(sAux4, sAux3);
					}
				}

				sDadosArqNovo += sLinha;
				sDadosArqNovo += "\r\n";
			}

			sLinha = sDadosArq.Tokenize("\r\n", iLinha);
		}

		int iPos = sFile.ReverseFind('.');
		int iQtdChar = sFile.GetLength()-iPos;
		CString sExtensao = sFile.Right(iQtdChar);

		sFile.Delete(iPos, iQtdChar);

		sFile += "Convertido";
		sFile += sExtensao;

		sDadosArqNovo.Trim();

		file.Open(sFile, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
		file.Write(sDadosArqNovo, sDadosArqNovo.GetLength());
		file.Close();
	}

	return 0;
}

