/*
 * vlist.c for elfsh
 * 
 * List all variables currently existing
 *
 * Updated Feb 07 2005 22:34:01 mayhem
 *
 */
#include "elfsh.h"



/* List the shell variables */
int		cmd_vlist()
{
  char		logbuf[BUFSIZ];
  hashent_t     *actual;
  u_int         index;
  u_int		cnt;
  elfshpath_t	*v;

  u_char	val8;
  u_short	val16;
  u_int		val32;
  elfsh_Addr	val64;
  char		*str;

  elfsh_SAddr	off;

  ELFSH_PROFILE_IN(__FILE__, __FUNCTION__, __LINE__);

  snprintf(logbuf, BUFSIZ, " [*] ELFsh currently existing variables : \n\n");
  vm_output(logbuf);

  for (cnt = 1, index = 0; index < vars_hash.size; index++)
    {
      for (actual = &vars_hash.ent[index]; 
	   actual != NULL && actual->key != NULL; 
	   actual = actual->next)
	{
	  v = actual->data;
	  switch (v->type)
	    {
	    case ELFSH_OBJBYTE:
	      val8 =  (v->immed ? v->immed_val.half : v->get_obj(v->parent));
	      printf(" [%02u] $%-20s Type:BYTE    = %02hhu         (%02hhd)       [%02hhX]   \n", 
		     cnt, actual->key, val8, val8, val8);
	      break;
	    case ELFSH_OBJSHORT:
	      val16 =  (v->immed ? v->immed_val.half : v->get_obj(v->parent));
	      printf(" [%02u] $%-20s Type:SHORT   = %04hu      (%04hd)       [%04hX] \n", 
		     cnt, actual->key, val16, val16, val16);
	      break;
	    case ELFSH_OBJINT:
	      val32 =  (v->immed ? v->immed_val.word : v->get_obj(v->parent));
	      printf(" [%02u] $%-20s Type:INTEGER = " UFMT10 " (" DFMT11 ") [" XFMT "] \n", 
		     cnt, actual->key, val32, val32, val32);
	      break;

	      /* Here needs to use vm_resolve */
	    case ELFSH_OBJLONG:
	      off = 0;
	      val64 =  (v->immed ? v->immed_val.ent : v->get_obj(v->parent));
	      str = vm_resolve(world.curjob->current, val64, &off);
	      if (!str)
		str = "?";
	      if (off)
		printf(" [%02u] $%-20s Type:LONG    = " UFMT10 " (" DFMT11 ") [" XFMT "] <%s + %d> \n", 
		       cnt, actual->key, val64, val64, val64, str, off);
	      else
		printf(" [%02u] $%-20s Type:LONG    = " UFMT10 " (" DFMT11 ") [" XFMT "] <%s> \n", 
		       cnt, actual->key, val64, val64, val64, str);
	      break;
	    case ELFSH_OBJSTR:
	      str = (v->immed ? v->immed_val.str : v->get_name(v->root, v->parent));
	      printf(" [%02u] $%-20s Type:STRING  = %s \n", cnt, actual->key, str);
	      break;
	    case ELFSH_OBJRAW:
	      str = (v->immed ? v->immed_val.str : v->get_name(v->root, v->parent));
	      printf(" [%02u] $%-20s Type:RAWDATA of length %u \n", cnt, actual->key, v->size);
	      break;
	    case ELFSH_OBJUNK:
	      printf(" [%02u] $%-20s Type:UNKNOWN \n", cnt, actual->key);
	      break;
	    default:
	      break;
	    }
	  cnt++;
	}
   }
  vm_output("\n");
  ELFSH_PROFILE_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}