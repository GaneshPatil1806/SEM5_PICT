if (i1 != -1)
            {
                if (i2 != -1)
                {
                    MDT mdte(mdtPtr, "\t" + word + "\t", "(P," + to_string(i1) + ") " + "(P," + to_string(i2) + ") ");
                    mdt[mdtPtr] = mdte;
                }
                else
                {
                    MDT mdte(mdtPtr, "\t" + word + "\t", "(P," + to_string(i1) + ") " + operand2);
                    mdt[mdtPtr] = mdte;
                }
                mdtPtr++;
            }